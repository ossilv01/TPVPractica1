//SOlucion:
//Cambiar Directorio de trabajo:
//: Solucion definitiva: $(ProjectDir)bin
//Actual:
//Anterior:$(ProjectDir)

//Error 2026 Proyecto
//Configuracion: Todas las config.
//Conjunto de herramientas de la plataforma: v145 for Microsoft C++ 

#include "game.h"
#include <iostream> //consola
#include <fstream> //lector archivo
#include <string>

#include <SDL3_image/SDL_image.h>

#include "Bow.h"
#include "Arrow.h"
#include "Balloon.h"
#include "texture.h"

#include <random> //semilla
#include <ctime>; // time()
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
	{"digits.png", 1, 10}

};

void
Game::loadLevel() {
	std::ifstream file("../assets/levels/1.txt");

	//Excepcion a quitar
	if (!file.is_open()) {
		std::cerr << "Error: No se pudo abrir el archivo de configuracion." << std::endl;
		return; // Si falla, se quedan los valores por defecto
	}

	string lector;
	while (file >> lector) {
		if (lector[0] == '#') {
			//ignorar
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else if (lector == "arrows") {
			file >> initialArrows;
		}
		else if (lector == "baldelay") {
			file >> baldelay;
		}
		else if (lector == "goal") {
			file >> goal;
		}
		else if (lector == "balpoints") {
			file >> balPoints;
		}
		else if (lector == "bgcolor") {
			file >> r;
			file >> g;
			file >> b;
		}
		else {
			// Palabra clave no reconocida: ignoramos el resto de la línea
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
	file.close();
	//cooldown cargado al temporizador
	spawnTimer = baldelay;
}

Game::Game()
  : exit(false), score(0)
{
	//Carga archivo
	loadLevel();

	// Carga SDL y crea la ventana con su renderizador
	if (!SDL_Init(SDL_INIT_VIDEO))
		throw "SDL_Init: "s + SDL_GetError();

	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE,
	                                 WINDOW_WIDTH, WINDOW_HEIGHT,
	                                 0, &window, &renderer))
		throw "window or renderer: "s + SDL_GetError();
		
	// Carga las texturas al inicio
	try {
		textures.fill(nullptr);
		string imgRoot = IMAGE_ROOT;

		for (size_t i = 0; i < textures.size(); i++) {
			auto [name, nrows, ncols] = textureList[i];
			textures[i] = new Texture(renderer, (imgRoot + name).c_str(), nrows, ncols);
		}
	}
	catch (...) {
		// TODO: liberar la memoria reservada
		for (auto tex : textures) {
			if (tex != nullptr)
			delete tex;
			tex = nullptr;
		}

		throw; // relanza la excepción
	}

	// Configura que se pueden utilizar capas translúcidas
	// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	
	// TODO: crear los objetos del juego
	//Creamos objeto arco
	bow = new Bow(Vector2D<float>(0, 0), Point2D<float>(10, 250),initialArrows, getTexture(BOW), this);

	//semilla generadora aleatoria
	random_device rd; 
	randomGenerator.seed(rd());
}

Game::~Game()
{
	// TODO: liberar la memoria reservada por la clase
	//Liberamos objeto bow y vectores objetos
	if (bow != nullptr) {
		delete bow;
		bow = nullptr;
	}
	for (Arrow* a : arrows) delete a;
	for (Balloon* b : balloons) delete b;
	//Libera array texturas
	for (Texture* texture : textures) {
		if (texture != nullptr)
		delete texture;
		texture = nullptr;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void
Game::render() const
{
	//0, 170, 85
	SDL_SetRenderDrawColor(renderer, r, g, b, 0);
	SDL_RenderClear(renderer);

	// TODO
	if (bow != nullptr) bow->render();
	for (Arrow* a : arrows) a->render();
	for (Balloon* b : balloons) b->render();
	//Puntuacion
	imprimirScore();

	SDL_RenderPresent(renderer);
}

void
Game::update(float delta)
{
	// TODO
	//Arco
	if (bow != nullptr) {
		bow->update(delta);
	}
	//Temporizador para generar globos uno a uno de forma constante
	//spawnTimer += delta;
	//if (spawnTimer >= SPAWN_COOLDOWN) {
	//	generateBalloon();
	//	spawnTimer = 0.0f; //Reiniciamos contador
	//}

	spawnTimer -= FRAME_PERIOD;
	if (spawnTimer <= 0) {
		generateBalloon();
		spawnTimer = baldelay;
	}

	//Flechas
	//bucle inverso para borrar de forma segura
	for (auto it = arrows.begin(); it != arrows.end(); ) {
		if (!(*it)->update(delta)) {
			delete* it;            // Liberamos memoria de la flecha
			it = arrows.erase(it); // La quitamos del vector y avanzamos el iterador
		}
		else {
			++it;
		}
	}

	// Globos
	for (auto it = balloons.begin(); it != balloons.end(); ) {
		//Muerto En general
		if (!(*it)->update(delta)) {
			//Si ha sido porque ha explotado
			if ((*it)->getterExplotado()) {
				score += balPoints; //puntuacion por globo
			}
			delete* it;
			it = balloons.erase(it);
		}
		else {
			++it;
		}
	}
	//Impresion en consola:
	imprimirResult();
}

void
Game::run()
{		
	Uint32 lastTime = SDL_GetTicks();
	while (!exit) {
		// TODO: implementar bucle del juego
		Uint32 startTime = SDL_GetTicks();
		float delta = (startTime - lastTime) / 1000.0f;
		lastTime = startTime;
		handleEvents();
		update(delta);
		render();

		//DELAY
		Uint32 frameTime = SDL_GetTicks() - startTime;
		if (frameTime < FRAME_PERIOD) {
			SDL_Delay(FRAME_PERIOD - frameTime);
		}

	}
}

void
Game::handleEvents()
{
	SDL_Event event;

	// Only quit is handled directly, everything else is delegated
	while (SDL_PollEvent(&event) &&!exit) {
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
	int posX = uniform_int_distribution<int>(WINDOW_WIDTH / 4, WINDOW_WIDTH - 100)(randomGenerator);
	int color = uniform_int_distribution<int>(0, 6)(randomGenerator);
	float vel = uniform_real_distribution<float>(1.5f, 4.0f)(randomGenerator);

	//Instancia globo
	//poscion de inicio, HEIGHT + 100 = por debajo del mapa
	// vector(0,-1) = dirección eje Y negativa (hacia arriba) * velocidad
	//textura + color (randomizado)
	Balloon* globo = new Balloon(Point2D<float>(posX, WINDOW_HEIGHT + 100),
		Vector2D<float>(0, -1) * vel, 
		getTexture(BALLOONS), 
		color);

	balloons.push_back(globo);
}

void
Game::imprimirResult() {
	if (bow != nullptr) {
		int flechasDisponibles = bow->getFlechas();
		static int ultimaCantidadFlechas = -1; // Guarda el estado del fotograma anterior

		// Solo imprimimos si el jugador ha disparado (el número cambió)
		if (flechasDisponibles != ultimaCantidadFlechas) {
			cout << "Flechas restantes: " << flechasDisponibles << endl;
			ultimaCantidadFlechas = flechasDisponibles;
		}

		// Si ya no quedan flechas en el arco Y todas las flechas disparadas han salido de la pantalla
		if (flechasDisponibles == 0 && arrows.empty()) {
			cout << "\n" << endl;
			cout << "FIN DEL JUEGO. Sin municion." << endl;
			cout << "Puntuacion final: " << score << " puntos." << endl;

			exit = true; // Rompe el bucle del Game::run() y cierra el juego limpiamente
		}
		// Superar juego en caso de que hay un goal que superar
		if (goal != 0 && score >= goal) {
			cout << "\n" << endl;
			cout << "HAS GANADO. FIN DEL JUEGO" << endl;
			cout << "Has llegado al goal de " << goal << " puntos" << endl;

			exit = true;
		}
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
			currentX -= (digitWidth + 2.0f);
			tempScore /= 10; // Eliminamos el último dígito ya pintado, 100 -> 10s
		}
	}
}


 bool
 Game::checkCollision(const Rectangle& rect)
 {
	// TODO
	 for (auto globo : balloons) {
		 if (globo != nullptr) {
			 if (globo->hit(rect)) {
				 return true; //Ha habido colision
			 }
		 }
	 }
	return false;
 }
