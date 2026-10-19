#include "game.h"

#include <string>

#include <SDL3_image/SDL_image.h>

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
		
	//Dibujar pantalla (color y temporizador) por ahora
		//SDL_RenderClear(renderer); 
		//SDL_RenderPresent(renderer); 
		//SDL_Delay(5000);		//Temporizador antes del crash 

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
		throw; // relanza la excepción
	}

	// Configura que se pueden utilizar capas translúcidas
	// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	
	// TODO: crear los objetos del juego



	//Finalizar programa
	SDL_DestroyRenderer(renderer);
	//SDL_DestroyWindow(window);
	//SDL_Quit();

}

Game::~Game()
{
	// TODO: liberar la memoria reservada por la clase
	/*for (SDL_Texture* texture : textures) {
		SDL_DestroyTexture(texture);
	}*/
}

void
Game::render() const
{
	SDL_RenderClear(renderer);

	// TODO
	/*for (int i = 0; i < textures.size(); i++)
		SDL_RenderTexture(renderer, textures[i]);*/

	SDL_RenderPresent(renderer);
}

void
Game::update()
{
	// TODO
}

void
Game::run()
{
	while (!exit) {
		// TODO: implementar bucle del juego
		//SDL_GetTicks();
		render();
		handleEvents();
		//update();
		//render();
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
		switch (event.type) {
		case SDL_EVENT_QUIT: exit = true; break;


		}
	}
}

// bool
// Game::checkCollision(const Rectangle& rect)
// {
//	// TODO
//	return false;
// }
