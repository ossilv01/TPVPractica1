//SOlucion:
//Cambiar Directorio de trabajo:C:\Users\kenai\Desktop\TPVPractica1\Arco&Flecha\bin\

//Actual:
//Anterior:$(ProjectDir)

//Error 2026 Proyecto
//Configuracion: Todas las config.
//Conjunto de herramientas de la plataforma: v145 for Microsoft C++ 

#include "game.h"

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

};

Game::Game()
  : exit(false)
{
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
	bow = new Bow(Vector2D<float>(0, 0), Point2D<float>(10, 250),10, getTexture(BOW), this);

	//semilla generadora aleatoria
	random_device rd; 
	//motor
	std::mt19937 gen(rd());

	//rango colores aleatorios
	int color = uniform_int_distribution<int>(0, 6)(gen);
	int color1 = uniform_int_distribution<int>(0, 6)(gen);
	int color2 = uniform_int_distribution<int>(0, 6)(gen);
	int color3 = uniform_int_distribution<int>(0, 6)(gen);

	//rango posiciones horizontales 
	int posX = uniform_int_distribution<int>(WINDOW_WIDTH/4, WINDOW_WIDTH-100)(gen);
	int posX1 = uniform_int_distribution<int>(WINDOW_WIDTH / 4, WINDOW_WIDTH-100)(gen);
	int posX2 = uniform_int_distribution<int>(WINDOW_WIDTH / 4, WINDOW_WIDTH-100)(gen);
	int posX3 = uniform_int_distribution<int>(WINDOW_WIDTH / 4, WINDOW_WIDTH-100)(gen);

	//rango velocidades globo
	float vel = uniform_real_distribution<float>(1.0f, 3.0f)(gen);
	float vel1 = uniform_real_distribution<float>(1.0f, 3.0f)(gen);
	float vel2 = uniform_real_distribution<float>(1.0f, 3.0f)(gen);
	float vel3 = uniform_real_distribution<float>(1.0f,3.0f)(gen);

		
	//Creacion ocasional de globos:
	Balloon* globo = new Balloon(Point2D<float>(posX, WINDOW_HEIGHT+100), Vector2D<float>(0, -1)*vel, getTexture(BALLOONS), color);
	balloons.push_back(globo);
	Balloon* globo2 = new Balloon(Point2D<float>(posX2, WINDOW_HEIGHT + 100), Vector2D<float>(0, -1)*vel1, getTexture(BALLOONS), color1);
	balloons.push_back(globo2);
	Balloon* globo3 = new Balloon(Point2D<float>(posX3, WINDOW_HEIGHT + 100), Vector2D<float>(0, -1)* vel2, getTexture(BALLOONS), color2);
	balloons.push_back(globo3);
	Balloon* globo4 = new Balloon(Point2D<float>(posX1, WINDOW_HEIGHT + 100), Vector2D<float>(0, -1)* vel3, getTexture(BALLOONS), color3);
	balloons.push_back(globo4);


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
	SDL_SetRenderDrawColor(renderer, 0, 170, 85, 0);
	SDL_RenderClear(renderer);

	// TODO
	if (bow != nullptr) bow->render();
	for (Arrow* a : arrows) a->render();
	for (Balloon* b : balloons) b->render();

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
		if (!(*it)->update(delta)) {
			delete* it;
			it = balloons.erase(it);
		}
		else {
			++it;
		}
	}
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
		// PUNTERO GAME A BOW PARA ACCEDER A CONSTANTE
		// HANDLE EVENTS SIMPLEMENTE SE LLAMA AL DE BOW 
		// NO MEZCLAR CODIGO 
		//Extra i guess
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

		//Detectar disparo
		if (event.type == SDL_EVENT_KEY_DOWN) {
			if (event.key.key == SDLK_D || event.key.key == SDLK_RIGHT) {
				if (bow != nullptr && bow->canShoot()) {
					//Punto referencia de flecha respecto a Arco (esquina)
					Point2D<float> arcoPos = bow->getPosition();

					//alturas arco 
					float a = 100;
					//Altura flecha
					float b = 82.0f / 9.0f;

					//Centrar flecha
					float flechaY = arcoPos.getY() + a/2 - b/2;
					//Nuevo punto centrado:
					Point2D<float> posCentro(arcoPos.getX(), flechaY);

					Arrow* newflecha = new Arrow(
						Vector2D<float>(1, 0),
						posCentro,
						getTexture(ARROW),
						this
					);
					arrows.push_back(newflecha);
					bow->Shoot();
				}
			}
		}
	}
}

 bool
 Game::checkCollision(const Rectangle& rect)
 {
	// TODO
	return false;
 }
