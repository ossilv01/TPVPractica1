#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include <istream>
#include <vector>
#include <random>

#include <list> //lista polimorfica
#include "rectangle.h"
//using namespace std;
// Declaraciones anticipadas
class Texture;
class Bow;
class Arrow;
class Balloon;
class SceneObject;
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

	static constexpr float VEL_MIN = 0.8f;
	static constexpr float VEL_MAX = 1.2f;

	//Posiciones y valores para Mariposas
	static constexpr float BUTTERFLY_X_MIN = WINDOW_WIDTH / 4;
	static constexpr float BUTTERFLY_X_MAX = WINDOW_WIDTH - 100;

	static constexpr float BUTTERFLY_Y_MIN = WINDOW_HEIGHT/6;
	static constexpr float BUTTERFLY_Y_MAX = WINDOW_HEIGHT - 150;

	static constexpr float ANGLE_MIN = 0.0f;
	static constexpr float ANGLE_MAX = 2.0f;

	enum TextureName
	{
		BOW = 0,
		ARROW,
		BALLOONS,
		DIGITS,
		BUTTERFLY,
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
	std::list<SceneObject*> sceneObjects; //Lista polimorifca unica
	Bow* bow = nullptr; // puntero a arco
	//std::vector<Arrow*> arrows; //Vector dinamico flechas
	////Globos
	//std::vector<Balloon*> balloons; //Vector dinamico de globos
	std::mt19937 randomGenerator;	//motor aletaorio
	//float spawnTimer = 0.0f;	//Cronometro tiempo para globos
	//static constexpr float SPAWN_COOLDOWN = 0.5f; //Frecuencia de generacion
	
	//Atributos de lectura de archivo
	int initialArrows = 8;
	int baldelay = 30; //delay en milisegundos
	int spawnTimer = 0; //cronometro en milisegundos
	int goal = 0;		//puntos para superar nivel
	int balPoints = 20; //puntos por globo
	int numButterflies = 0; //Numero mariposasa
	float balspeed; // nose

	int r = 0;//color fondo
	int g = 0;
	int b = 0; 

	//Debido a que ya no existe un vector de arrows sino una lista de gameobjects
	//Utilizo un contador para saber cuantas flechas activas en el juego hay
	int numFlechasActivas = 0;

	int ultimaCantidadFlechas = -1;

	int butterflyPoints = 5;
	//Metodo auxiliar para generar globos y mariposas
	void generateBalloon();
	void generateButterfly();

	//Metodo auxiliar para imprimir nºflechas en consola:
	void imprimirResult();
	//Metodo auxiliar para imprimir Puntuacion en el juego
	void imprimirScore() const;

	//Lectura de archivo
	bool loadLevel(int level);
	int currentLevel = 1; //Se incia en el 1 por el enunciado
	void nextLevel(); //Preparar el siguiente nivel

	//Limpieza texturas y sdl
	void clean(); //memoryleaks
	void init();
	void clearScene(); 

	//Guardado y carga de momento de partida
	void saveGame(const std::string& filename) const;
	void loadGame(const std::string& filename);
	//Carga individual de cada objeto
	SceneObject* loadObject(char tipo, std::istream& in);

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
	 void addArrow(Arrow* arrow);
	 void removeArrow() {
		 if (numFlechasActivas > 0) numFlechasActivas--;
	 }

	 //Devuelve angulo aleatorio (para butterfly)
	 float getRandomAngle();

	 //Metodo para Balloon y Butterfly, se encarga de sumar o restar puntos
	 void addScore(int points) {
		 score += points;
		 if (score < 0) score = 0;
	 }
};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

#endif // GAME_H
