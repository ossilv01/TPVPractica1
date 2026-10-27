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
//semilla
#include <random> 
// time() 
#include <ctime>; 
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
	std::ifstream file("../assets/levels/2.txt");

	if (!file.is_open()) {
		throw string("Error: No se puede abrir el archivo de configuracion del nivel");
	}

	string lector;
	while (file >> lector) {
		if (lector[0] == '#') {
			//ignorar comentarios #
			file.ignore(numeric_limits<std::streamsize>::max(), '\n');
		}
		else if (lector == "arrows") {
			if (!(file >> initialArrows)) {
				throw string("Error: formato invalido para 'arrows' en archivo de nivel.");
			}
		}
		else if (lector == "baldelay") {
			if (!(file >> baldelay)) {
				throw string("Error: formato invalido para 'baldelay' en archivo de nivel");
			}
		}
		else if (lector == "goal") {
			if (!(file >> goal)) {
				throw string("Error: formato invalido para 'goal' en archivo de nivel");
			}
		}
		else if (lector == "balpoints") {
			if (!(file >> balPoints)) {
				throw string("Error: formato invalido para 'balpoints' en archivo de nivel");
			}
		}
		else if (lector == "bgcolor") {
			if (!(file >> r >> g >> b)) {
				throw string("Error: formato invalido para 'bgcolor' en archivo de nivel.");
			}
		}
		else {
			// Palabra clave no reconocida: ignoramos el resto de la línea
			file.ignore(numeric_limits<std::streamsize>::max(), '\n');
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
			string fullPath = imgRoot + name;

			textures[i] = new Texture(renderer, (imgRoot + name).c_str(), nrows, ncols);
			if (textures[i] == nullptr) {
				throw string("Error: Fichero de imagen no encontrado o valido para:" + fullPath);
			}
		}
	}

	catch (const string& e) {
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
	bow = new Bow(Vector2D<float>(0, 0), Point2D<float>(BOW_X, BOW_Y),initialArrows, getTexture(BOW), this);

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

	spawnTimer -= FRAME_PERIOD;
	if (spawnTimer <= 0) {
		generateBalloon();
		spawnTimer = baldelay;
	}

	//Flechas
	//bucle para borrar de forma segura
	for (auto it = arrows.begin(); it != arrows.end(); ) {
		if (!(*it)->update(delta)) {
			delete* it;            // Liberamos memoria de la flecha
			it = arrows.erase(it); // La quitamos del vector y avanzamos el iterador
		}
		else {
			++it;
		}
	}

	////bucle inverso de verdad
	//for (int i = arrows.size() - 1; i >= 0; --i) {
	//	if (!arrows[i]->update(delta)) {
	//		delete arrows[i];
	//		arrows.erase(arrows.begin() + i); // Al ir al revés, no desordena los índices anteriores
	//	}
	//}

	//bucle normal sin .begin () y .end() 
	//for (int i = 0; i < arrows.size();) {
	//	if (!arrows[i]->update(delta)) {
	//		delete arrows[i];
	//		arrows.erase(arrows.begin() + i);
	//	}
	//	else {
	//		i++;
	//	}
	//}

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
		//Vemos cuanto ha tardado el frame
		//SI ha tardado menos del objetivo dormimos el procesador el tiempo restante
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
	int posX = uniform_int_distribution<int>(BALLOON_X_MIN, BALLOON_X_MAX)(randomGenerator);
	int color = uniform_int_distribution<int>(0, 6)(randomGenerator);
	float vel = uniform_real_distribution<float>(VEL_MIN, VEL_MAX)(randomGenerator);

	//Instancia globo
	//poscion de inicio, HEIGHT + 100 = por debajo del mapa
	// vector(0,-1) = dirección eje Y negativa (hacia arriba) * velocidad
	//textura + color (randomizado)
	Balloon* globo = new Balloon(Point2D<float>(posX, BALLOON_Y),
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
			currentX -= (digitWidth + DIGIT_MARGIN_PLUS);
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
