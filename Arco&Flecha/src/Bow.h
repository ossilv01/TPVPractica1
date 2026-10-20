#include "SDL3/SDL.h"
#include "texture.h"
#include "vector2D.h"
#include "rectangle.h"

class Bow
{
private:
	//puntero a textura
	Texture* tex = nullptr; 
	//Armado o no
	bool armed = false; 
	//Direccion movimienot
	Vector2D<float> dir;
	//Dimensiones 
	Rectangle tam;
	//numero flechas
	int flechas; 
	//Velocidad escalar
	float velocidadEscalar = 300.0f;


public:
	//Constructor:
	Bow(Vector2D<float>, Point2D<float>, int , Texture*);
	//Metodos
	void handleEvent(const SDL_Event& event);
	void render() const;
	bool update(float delta, float windowHeight);
	//El equivalente a las vidas
	int getFlechas() const;

};

