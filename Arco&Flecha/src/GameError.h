#pragma once
#include <stdexcept>
#include <string>
#include <SDL3/SDL.h>
//Superclase base: hereda de logic_error
class GameError : public std::logic_error
{
public:
	using std::logic_error::logic_error; //hereda de constructor
};

//Error de SDL
class SDLError : public GameError {
public:
	SDLError(const std::string& message = "")
		: GameError(message + (message.empty() ? "" : ": ") + SDL_GetError()) {
	}
};

//Archivo no encontrado 
class FileNotFoundError : public GameError {
public:
	FileNotFoundError(const std::string& filename)
		: GameError("Fichero no encontrado: " + filename) {
	}
};

//Error formato lectura de archivo
class FileFormatError : public GameError {
public:
	FileFormatError(const std::string& filename, int line, const std::string& message)
		: GameError("Error de formato en '" + filename + "' (linea " + std::to_string(line) + "): " + message) {
	}

};
