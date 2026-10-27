#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include <istream>
#include <vector>
#include <random>

#include "rectangle.h"
//using namespace std;
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

	//Constantes texto puntuacion: 
	static constexpr float DIGIT_WIDTH = 24.0f;
	static constexpr float  DIGIT_HEIGHT = 32.0f;
	static constexpr float  DIGIT_MARGIN_PLUS = 2.0f;
	static constexpr float  DIGIT_X = WINDOW_WIDTH- DIGIT_WIDTH- 20.0f;
	static constexpr float  DIGIT_Y = 20;

	//Posicion Inicial Arco: 
	static constexpr float BOW_X = 10.0f;
	static constexpr float BOW_Y = WINDOW_HEIGHT / 2 - 50;

	//Posicion Globos: 
	static constexpr float BALLOON_Y = WINDOW_HEIGHT + 100;
	static constexpr float BALLOON_X_MIN = WINDOW_WIDTH / 4;
	static constexpr float BALLOON_X_MAX = WINDOW_WIDTH - 100;
	//Generacion globos: 
	static constexpr float VEL_MIN = 1.5f;
	static constexpr float VEL_MAX = 4.0f;


	enum TextureName
	{
		BOW = 0,
		ARROW,
		BALLOONS,
		DIGITS,
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
	std::vector<Arrow*> arrows; //Vector dinamico flechas
	//Globos
	std::vector<Balloon*> balloons; //Vector dinamico de globos
	std::mt19937 randomGenerator;	//motor aletaorio
	//float spawnTimer = 0.0f;	//Cronometro tiempo para globos
	//static constexpr float SPAWN_COOLDOWN = 0.5f; //Frecuencia de generacion
	
	//Atributos de lectura de archivo
	int initialArrows = 8;
	int baldelay = 30; //delay en milisegundos
	int spawnTimer = 0; //cronometro en milisegundos
	int goal = 0;		//puntos para superar nivel
	int balPoints = 20; //puntos por globo

	int r = 0;//color fondo
	int g = 0;
	int b = 0; 


	//Metodo auxiliar para generar globos
	void generateBalloon();

	//Metodo auxiliar para imprimir nºflechas en consola:
	void imprimirResult();
	//Metodo auxiliar para imprimir Puntuacion en el juego
	void imprimirScore() const;

	void loadLevel();

	//Puntuacion que se actualiza
	int score = 0;

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
