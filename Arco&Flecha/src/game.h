#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include <istream>
#include <vector>
#include <random>

#include "rectangle.h"
using namespace std;
// Declaraciones anticipadas
class Texture;
class Bow;
class Arrow;
class Balloon;
/**
 * Clase principal del juego.
 */
class Game
{
public:
	// Número de actualizaciones por segundo
	static constexpr int FRAME_RATE = 30;
	// Periodo entre actualizaciones en milisegundos
	static constexpr int FRAME_PERIOD = 1000 / FRAME_RATE;
	// Tamaño real de la ventana
	static constexpr int WINDOW_WIDTH = 800;
	static constexpr int WINDOW_HEIGHT = 600;

	enum TextureName
	{
		BOW = 0,
		ARROW,
		BALLOONS,
		NUM_TEXTURES,

	};

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::array<Texture*, NUM_TEXTURES> textures;

	void render() const;
	void update(float delta);
	void handleEvents();

	bool exit;

	// Elemento del juego
	// TODO: añadir atributos para los objetos del juego
	Bow* bow = nullptr; // puntero a arco
	vector<Arrow*> arrows; //Vector dinamico flechas
	//Globos
	vector<Balloon*> balloons; //Vector dinamico de globos
	mt19937 randomGenerator;	//motor aletaorio
	float spawnTimer = 0.0f;	//Cronometro tiempo para globos
	static constexpr float SPAWN_COOLDOWN = 0.5f; //Frecuencia de generacion

	//Metodo auxiliar para generar globos
	void generateBalloon();



public:
	Game();
	~Game();

	// Obtiene una textura por su nombre
	Texture* getTexture(TextureName name) const;

	// Ejecuta el bucle principal del juego
	void run();

	// Comprueba si hay algún objeto colocado en ese rectángulo
	 bool checkCollision(const Rectangle& rect);
	 
	 //Metodo para instnaciar creacion flecha
	 void addArrow(Arrow* arrow) {
		 arrows.push_back(arrow);
	 }
};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

#endif // GAME_H
