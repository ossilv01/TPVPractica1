//SOlucion:
//Cambiar Directorio de trabajo:
//: Solucion definitiva: $(ProjectDir)bin
//Actual:
//Anterior:$(ProjectDir)

//Error 2026 Proyecto
//Configuracion: Todas las config.
//Conjunto de herramientas de la plataforma: v145 for Microsoft C++ 

#include "game.h"
//consola
#include <iostream> 
//lector archivo
#include <fstream> 
#include <string>

#include <SDL3_image/SDL_image.h>

#include "Bow.h"
#include "Arrow.h"
#include "Balloon.h"
#include "texture.h"
#include "Butterfly.h"

//nuevos
#include "SceneObject.h"
#include "MobileObject.h"
#include "GameObject.h"
#include "GameError.h"

//semilla
#include <random> 
// time() 
#include <ctime> 
using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Bow & Arrow 1.0";
constexpr const char* IMAGE_ROOT = "../assets/images/";
constexpr const char* LEVEL_ROOT = "../assets/levels/";

// Estructura para especificar las texturas que hay que
// cargar y el tamaño de su matriz de frames
struct TextureSpec
{
	const char* name;
	int nrows = 1;
	int ncols = 1;
};

constexpr array<TextureSpec, Game::NUM_TEXTURES> textureList{
	TextureSpec{"bow.png", 1, 2},
	{"arrow.png"},
	{"balloons.png", 7, 6},
	{"digits.png", 1, 10},
	{"butterfly.png", 4, 10}

};

bool
Game::loadLevel(int level) {
	string path = LEVEL_ROOT + to_string(level) + ".txt";
	std::ifstream file(path);

	//Si archivo no existe devolvemos false. Casos a proecesar mas abajo:
	if (!file.is_open()) {
		return false;
	}
	//Se inicializa goal en caso de que no lo tenga el nivel
	goal = 0; 
	numButterflies = 0; //por ahora
	string lector;
	int lineNumber = 0;
	while (file >> lector) {
		lineNumber++;
		string error = "";

		if (lector[0] == '#') {
			//ignorar comentarios #
			file.ignore(numeric_limits<std::streamsize>::max(), '\n');
		}
		else if (lector == "arrows") {
			file >> initialArrows;
			error = lector;
		}
		else if (lector == "baldelay") {
			file >> baldelay;
			error = lector;
		}
		else if (lector == "goal") {
			file >> goal;
			error = lector;
		}
		else if (lector == "balpoints") {
			file >> balPoints;
			error = lector;
		}
		else if (lector == "bgcolor") {
			file >> r >> g >> b;
			error = lector;
		}
		else if (lector == "butterflies") {
			file >> numButterflies;
			error = lector;
		}
		else if (lector == "balspeed") {
			file >> balspeed;
			error = lector;
		}
		else {
			// Palabra clave no reconocida: ignoramos el resto de la línea
			file.ignore(numeric_limits<std::streamsize>::max(), '\n');
		}
		if (!error.empty() && file.fail()) {
			//throw string("Error: formato invalido para '" + error + "' en archivo " + path);
			throw FileFormatError(path, lineNumber, "Formato invalido para: '" + lector + "'");
		}
	}
	file.close();
	//cooldown cargado al temporizador
	spawnTimer = baldelay;
	return true;
}


Game::Game()
  : exit(false), score(0)
{
	//Carga archivo
	if (!loadLevel(currentLevel)) {
		//throw string("Error: No se puede abrir el archivo de configuracion del nivel 1.txt");
		throw FileNotFoundError(string(LEVEL_ROOT) + "1.txt");
	}

	// Carga SDL y crea la ventana con su renderizador
	if (!SDL_Init(SDL_INIT_VIDEO))
		//throw "SDL_Init: "s + SDL_GetError();
		throw SDLError("Error al Incializar SDL");

	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE,
	                                 WINDOW_WIDTH, WINDOW_HEIGHT,
	                                 0, &window, &renderer))
		//throw "window or renderer: "s + SDL_GetError();
		throw SDLError("Error al crear ventana/render");
	// Carga las texturas al inicio
	try {
		textures.fill(nullptr);
		string imgRoot = IMAGE_ROOT;

		for (size_t i = 0; i < textures.size(); i++) {
			auto [name, nrows, ncols] = textureList[i];
			string fullPath = imgRoot + name;

			textures[i] = new Texture(renderer, (imgRoot + name).c_str(), nrows, ncols);
			if (textures[i] == nullptr) {
				//throw string("Error: Fichero de imagen no encontrado o valido para:" + fullPath);
				throw FileNotFoundError(fullPath);
			}
		}
	}

	catch (const exception& e) {
		// TODO: liberar la memoria reservada
		clean();
		throw; // relanza la excepción
	}

	// Configura que se pueden utilizar capas translúcidas
	// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	
	// TODO: crear los objetos del juego
	////Creamos objeto arco
	//bow = new Bow(Vector2D<float>(0, 0), Point2D<float>(BOW_X, BOW_Y),initialArrows, getTexture(BOW), this);
	//sceneObjects.push_back(bow); //añadido a la lista bow
	//semilla generadora aleatoria
	random_device rd; 
	randomGenerator.seed(rd());
	//Preparacion nivel
	init();
}

void Game::clean() {
	
	//Liberamos objeto bow y vectores objetos
	//if (bow != nullptr) {
	//	delete bow;
	//	bow = nullptr;
	////}
	//for (Arrow* a : arrows) delete a;
	//arrows.clear();
	//for (Balloon* b : balloons) delete b;
	//balloons.clear();

	//Adaptado a la lista polimorfica
	for (SceneObject* obj : sceneObjects) delete obj;
	sceneObjects.clear();
	bow = nullptr;

	//Libera array texturas
	for (Texture* texture : textures) {
		if (texture != nullptr)
			delete texture;
		texture = nullptr;
	}
	//Liberamos elementos SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

Game::~Game()
{
	// TODO: liberar la memoria reservada por la clase
	clean();
}

void
Game::render() const
{
	//0, 170, 85
	SDL_SetRenderDrawColor(renderer, r, g, b, 0);
	SDL_RenderClear(renderer);

	// TODO
	//if (bow != nullptr) bow->render();
	//for (Arrow* a : arrows) a->render();
	//for (Balloon* b : balloons) b->render();
	//Unica render
	for (SceneObject* obj : sceneObjects) obj->render();

	//Puntuacion
	imprimirScore();

	SDL_RenderPresent(renderer);
}

void
Game::update(float delta)
{
	// TODO
	// Restamos los milisegundos fijos que dura cada frame gracias al SDL_Delay
	spawnTimer -= FRAME_PERIOD;
	if (spawnTimer <= 0) {
		generateBalloon();
		spawnTimer = baldelay; // Se reinicia al delay dado 
	}

	//Updates de cada sceneObject
	for (auto it = sceneObjects.begin(); it != sceneObjects.end();) {
		if (!(*it)->update(delta)) {
			delete* it;
			it = sceneObjects.erase(it);
		}
		else {
			++it;
		}
	}

	//Impresion en consola:
	imprimirResult();

	//Caso sin municion = Derrota
	if (bow != nullptr && bow->getFlechas() == 0 && numFlechasActivas == 0) {
		cout << "\nFIN DEL JUEGO. Sin municion." << endl;
		cout << "Puntuacion final: " << score << " puntos." << endl;
		exit = true;
	}
	// Ver si hay mas niveles, para cambiar
	else if (goal != 0 && score >= goal) {
		nextLevel();
	}
}

void
Game::run()
{		
	Uint32 lastTime = SDL_GetTicks();
	while (!exit) {
		// TODO: implementar bucle del juego
		//Medicion interna de Cuanto tarda en procesar el fotograma actual -> eventos, update y render
		//Rendimiento PC
		Uint32 startTime = SDL_GetTicks();
		//Delta: Mide en tiempo real cuantos segundos pasaron desde la ultima vez que se actualizaron
		float delta = (startTime - lastTime) / 1000.0f;
		lastTime = startTime;
		handleEvents();
		update(delta);
		render();

		//DELAY
		//Vemos cuanto ha tardado el frame
		//SI ha tardado menos del objetivo dormimos el procesador el tiempo restante
//ejemplo: 1005 (tiempo actual en este instante, tras terminar tareas, update)	startTime = 1000 (tiempo que haya tardado justo al empezar las tareas)
		Uint32 frameTime = SDL_GetTicks() - startTime;
		//procesaria todo en 5 milisegundos, cuando en realidad el objetivo eran 33 milisegundos
		if (frameTime < FRAME_PERIOD) {
			//por lo tanto usamos delay de 28 milisegundos para no ir tan rapido
			SDL_Delay(FRAME_PERIOD - frameTime);
		}

	}
}


void
Game::saveGame(const std::string& filename) const {
	std::ofstream out(filename);
	if (!out.is_open()) {
		throw GameError("No se pudo crear el archivo de guardado: " + filename);
	}
	//Datos globales
	out << currentLevel << " " << score << " "<< spawnTimer << endl;

	//Guardado de todos los objetos de la escena
	for (const SceneObject* obj : sceneObjects) {
		obj->save(out);
	}
	out.close();
}

void 
Game::loadGame(const std::string& filename) {
	std::ifstream in(filename);
	if (!in.is_open()) {
		throw FileNotFoundError(filename);
	}
	//Lectura valores globales
	int savedLevel, savedScore;
	float savedTimer;
	in >> savedLevel >> savedScore >> savedTimer;
	if (in.fail()) {
		throw FileFormatError(filename, 1, "Cabecera de partida guardada invalida");
	}
	//Cargamos parametros del nivel correspondiente
	if (!loadLevel(savedLevel)) {
		throw FileFormatError(filename, 1, "El nivel guardado (" + std::to_string(savedLevel) + ") no existe");
	}
	//Limpieza e incializacion
	for (SceneObject* obj : sceneObjects) {
		delete obj;
	}
	sceneObjects.clear();
	bow = nullptr;
	numFlechasActivas = 0;

	currentLevel = savedLevel;
	score = savedScore;
	spawnTimer = savedTimer;
	//Leer cada objeto
	char tipo;
	int lineNumber = 1;
	while (in >> tipo) {
		lineNumber++;
		if (tipo == 'W') {
			bow = new Bow(Vector2D<float>(0, 0), Point2D<float>(0, 0), 0, getTexture(BOW), this);
			bow->load(in);
			sceneObjects.push_back(bow);
			ultimaCantidadFlechas = bow->getFlechas();
		}
		else if (tipo == 'A') {
			Arrow* arrow = new Arrow(Vector2D<float>(0, 0), Point2D<float>(0, 0), getTexture(ARROW), this);
			arrow->load(in);
			sceneObjects.push_back(arrow);
			numFlechasActivas++;
		}
		else if (tipo == 'B') {
			Balloon* balloon = new Balloon(Point2D<float>(0, 0), Vector2D<float>(0, 0), getTexture(BALLOONS), 0, balPoints, this);
			balloon->load(in);
			sceneObjects.push_back(balloon);
		}
		else if (tipo == 'M') {
			Butterfly* butterfly = new Butterfly(this, Point2D<float>(0, 0), Vector2D<float>(0, 0), getTexture(BUTTERFLY), butterflyPoints);
			butterfly->load(in);
			sceneObjects.push_back(butterfly);
		}
		else {
			throw FileFormatError(filename, lineNumber, std::string("Tipo de objeto desconocido: ") + tipo);
		}
		if (in.fail()) {
			throw FileFormatError(filename, lineNumber, "Datos corruptos o incompletos al cargar objeto");
		}
	}
	in.close();
}


void
Game::handleEvents()
{
	SDL_Event event;

	// Only quit is handled directly, everything else is delegated
	string path = string(LEVEL_ROOT) + "saved.txt";
	while (SDL_PollEvent(&event) &&!exit) {
		if (event.type == SDL_EVENT_KEY_DOWN) {
			if ((event.key.mod & SDL_KMOD_CTRL) != 0) {
				if (event.key.key == SDLK_S) {
					saveGame(path);
					cout << "Partida guardada en saved.txt" << endl;
				}
				else if (event.key.key == SDLK_O) {
					loadGame(path);
					cout << "Partida cargada desde saved.txt" << endl;
				}
			}
		}

		if (event.type == SDL_EVENT_QUIT)
			exit = true;
		// TODO
		//Llamamos al handleEvent de bow (movimiento y demas)
		if (bow != nullptr) {
			bow->handleEvent(event);
		}
	}
}

void
Game::generateBalloon() {
	//Elementos random
	// Posicion, color y velocidad
	int posX = uniform_int_distribution<int>(BALLOON_X_MIN, BALLOON_X_MAX)(randomGenerator);
	int color = uniform_int_distribution<int>(0, 6)(randomGenerator);
	float factorAleatorio = uniform_real_distribution<float>(VEL_MIN, VEL_MAX)(randomGenerator);
	float vel = (balspeed * 1000.0f) * factorAleatorio; //0.2 *1000 = 200 *factor aleatorio
	//Instancia globo
	//poscion de inicio, HEIGHT + 100 = por debajo del mapa
	// vector(0,-1) = dirección eje Y negativa (hacia arriba) * velocidad
	//textura + color (randomizado)
	//puntos por globo + puntero a Game
	Balloon* globo = new Balloon(Point2D<float>(posX, BALLOON_Y),
		Vector2D<float>(0, -1) * vel, 
		getTexture(BALLOONS), 
		color,
		balPoints,
		this);

	sceneObjects.push_back(globo);
}

void
Game::generateButterfly() {
	int posX = uniform_int_distribution<int>(BUTTERFLY_X_MIN, BUTTERFLY_X_MAX)(randomGenerator);
	int posY = uniform_int_distribution<int>(BUTTERFLY_Y_MIN, BUTTERFLY_Y_MAX)(randomGenerator);

	//angulo aleatorio y puesto en direccion
	float angle = uniform_real_distribution<float>(0.0f, 2.0f * 3.14159f)(randomGenerator);
	Vector2D<float> dir(std::cos(angle), std::sin(angle));

	Butterfly* mariposa = new Butterfly(this, Point2D<float>(posX, posY),
		dir,
		getTexture(BUTTERFLY),
		butterflyPoints
	);
	sceneObjects.push_back(mariposa);
}


void
Game::imprimirResult() {
	if (bow != nullptr) {
		int flechasDisponibles = bow->getFlechas();

		// Solo imprimimos si el jugador ha disparado (el número cambió)
		if (flechasDisponibles != ultimaCantidadFlechas) {
			cout << "Flechas restantes: " << flechasDisponibles << endl;
			ultimaCantidadFlechas = flechasDisponibles;
		}
	}
}

void Game::nextLevel() {
	//Si no existe el nivel siguiente = victoria
	//si es que tiene el score puesto para un valor, si no, es como nivel infinito
	//jugar hasta que pierdas
	currentLevel++;
	if (!loadLevel(currentLevel)) {
		cout << "\n" << endl;
		cout << "HAS GANADO. FIN DEL JUEGO" << endl;
		cout << "HAS SUPERADO TODOS LOS NIVELES. PUNTUACIÓN: " << goal << " puntos" << endl;
		exit = true;
	}
	//Nuevo nivel
	else {
		cout << "\n" << "Nivel: " << currentLevel << endl;
		score = 0;
		init();
		//Borrar globos,flecha y arco
		//for (SceneObject* obj : sceneObjects) {
		//	delete obj;
		//}
		//sceneObjects.clear();
		//bow = nullptr;
		//numFlechasActivas = 0;

		//// Reincio marcadores del nivel y flehcas
		//score = 0;
		//ultimaCantidadFlechas = initialArrows;
		//
		////Nuevo arco
		//bow = new Bow(Vector2D<float>(0, 0), Point2D<float>(BOW_X, BOW_Y), initialArrows, getTexture(BOW), this);
		//sceneObjects.push_back(bow);
	}
}


void
Game::imprimirScore() const{
	Texture* digitsTex = getTexture(DIGITS);
	if (digitsTex != nullptr) {
		int tempScore = score;

		//ancho y alto de cada cifra en pantalla
		float digitWidth = DIGIT_WIDTH;
		float digitHeight = DIGIT_HEIGHT;

		// margen inicial desde el borde derecho de la ventana
		float currentX = DIGIT_X;
		float currentY = DIGIT_Y;

		// Pintamos 0 inicial
		if (tempScore == 0) {
			SDL_FRect destRect = { currentX, currentY, digitWidth, digitHeight };
			digitsTex->renderFrame(destRect, 0, 0); //fila 0, columna 0
		}

		// Descomponemos el número dígito por dígito cuando reciba puntuacion
		while (tempScore > 0) {
			int digit = tempScore % 10; // Para empezar obtenemos el último dígito. 100 -> 0
			SDL_FRect destRect = { currentX, currentY, digitWidth, digitHeight };

			// Dibujamos el dígito correspondiente usando la columna de la textura
			digitsTex->renderFrame(destRect, 0, digit);

			// Desplazamos la posición X hacia la izquierda para el siguiente dígito
			currentX -= (digitWidth + DIGIT_MARGIN_PLUS);
			tempScore /= 10; // Eliminamos el último dígito ya pintado, 100 -> 10s
		}
	}
}

//Arrancar cualquier nivel
void 
Game::init() {
	//Limpiamos 
	for (SceneObject* obj : sceneObjects) {
		delete obj;
	}
	sceneObjects.clear();
	bow = nullptr;
	numFlechasActivas = 0;

	// Reincio marcadores del nivel y flehcas
	score = 0;
	ultimaCantidadFlechas = initialArrows;

	//Nuevo arco
	bow = new Bow(Vector2D<float>(0, 0), Point2D<float>(BOW_X, BOW_Y), initialArrows, getTexture(BOW), this);
	sceneObjects.push_back(bow);
	//Mariposas de una vez
	for (int i = 0; i < numButterflies; i++) {
		generateButterfly();
	}
}

 bool
 Game::checkCollision(const Rectangle& rect)
 {
	// TODO
	 //for (auto globo : balloons) {
		// if (globo != nullptr) {
		//	 if (globo->hit(rect)) {
		//		 return true; //Ha habido colision
		//	 }
		// }
	 //}
	 for (SceneObject* obj : sceneObjects) {
		 if (obj != nullptr && obj->hit(rect))
		 {
			 return true; // ha habido colision
		 }
		}
	return false;
 }

 void Game::addArrow(Arrow* arrow) {
	 sceneObjects.push_back(arrow);
	 numFlechasActivas++;
 }
