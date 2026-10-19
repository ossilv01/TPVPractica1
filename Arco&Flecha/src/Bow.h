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
	//Velocidad/Direccion
	Vector2D<float> move;
	//Posicion
	Point2D<float> pos;
	//Dimensiones
	Rectangle tam {pos, 402, 381};

	//numero flechas
	int flechas; 


public:
	//Constructor:
	Bow(Vector2D<float>, Point2D<float>, int , Texture*, Rectangle);
	//Metodos
	void handleEvent(const SDL_Event& event);
	void render();
	bool update(float delta);
	//El equivalente a las vidas
	int getFlechas() const;

};

