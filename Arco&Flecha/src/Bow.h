#include "SDL3/SDL.h"
#include "texture.h"
#include "vector2D.h"
#include "rectangle.h"

class Game;
class Bow
{
private:

	//dimensiones
	static constexpr float altura = 100.0f;
	static constexpr float anchura = 100.0f;

	//puntero a textura
	Texture* tex = nullptr; 
	//puntero a game
	Game* game = nullptr;
	//Armado o no
	bool armed; 
	//Direccion movimienot
	Vector2D<float> dir;
	//Dimensiones 
	Rectangle tam;
	//numero flechas
	int flechas = 10; 
	//Velocidad escalar
	float velocidadEscalar = 300.0f;


public:
	//Constructor:
	Bow(Vector2D<float>, Point2D<float>, int , Texture*, Game*);
	//Metodos
	void handleEvent(const SDL_Event& event);
	void render() const;
	bool update(float delta);
	//El equivalente a las vidas
	int getFlechas() const;

	//Metodos para disparo
	bool canShoot() const;
	void Shoot();
	//getter de Point2D
	Point2D <float> getPosition();

};

