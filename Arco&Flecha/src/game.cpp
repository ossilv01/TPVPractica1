//SOlucion:
//Cambiar Directorio de trabajo:C:\Users\kenai\Desktop\TPVPractica1\Arco&Flecha\bin\

//Actual:
//Anterior:$(ProjectDir)

#include "game.h"

#include <string>

#include <SDL3_image/SDL_image.h>

#include "Bow.h"
#include "Arrow.h"
#include "Balloon.h"

#include "texture.h"

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
		for (auto tex : textures) delete tex;
		throw; // relanza la excepción
	}

	// Configura que se pueden utilizar capas translúcidas
	// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	
	// TODO: crear los objetos del juego
	//Creamos objeto arco
	bow = new Bow(Vector2D<float>(0, 0), Point2D<float>(10, 250),10, getTexture(BOW));

}

Game::~Game()
{
	// TODO: liberar la memoria reservada por la clase
	//Liberamos objeto bow y vectores objetos
	delete bow;
	for (Arrow* a : arrows) delete a;
	for (Balloon* b : balloons) delete b;
	//Libera array texturas
	for (Texture* texture : textures) delete texture;

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
		bow->update(delta, WINDOW_HEIGHT);
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
	}
}

 bool
 Game::checkCollision(const Rectangle& rect)
 {
	// TODO
	return false;
 }
