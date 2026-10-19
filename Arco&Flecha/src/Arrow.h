#include "vector2D.h"
#include "rectangle.h"
#include "texture.h"

class Game;

class Arrow
{
private:
	//Puntero textura
	Texture* tex = nullptr; 

	//Puntero a game para colisiones (proximo) 
	//posible dependencia circular? 
	Game* game = nullptr;

	//Dimensiones:
	Rectangle tam;

	//Velocidad/Direccion movimiento: 
	Vector2D <float> dir;

	//velocidad escalar:
	float Velocidad = 500.0f;

public:
	//Constructor
	Arrow(Vector2D<float>, Point2D<float>, Texture*, Game* game);

	void render() const;
	bool update(float delta);
};

