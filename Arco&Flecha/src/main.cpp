//
// Tecnología de la Programación de Videojuegos 1
// Facultad de Informática UCM
//
// Plantilla de proyecto con SDL
//
#include <iostream>
#include "game.h"
#include "SDL3/SDL.h"
#include "GameError.h"

int main(int argc, char* argv[])
{
	try {
		Game().run();
	}
	// TODO: manejar excepciones
	catch (const GameError& e) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error del juego", e.what(), nullptr);
		std::cerr << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error Inesperado", e.what(), nullptr);
		std::cerr << e.what() << std::endl;

		//std::cout << "ERROR CRITICO: " << e << std::endl;
		return 1;
	}
	return 0;
}
