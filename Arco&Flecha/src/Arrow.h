#include "vector2D.h"
#include "rectangle.h"
#include "texture.h"
#include "game.h"
class Arrow
{
private:
	//Puntero textura
	Texture* tex = nullptr; 

	//Puntero a game para colisiones (proximo) 
	//posible dependencia circular? 
	//Game* game;

	//posicion
	Point2D<float> pos; 

	//Dimensiones:
	Rectangle tam{ pos, 404, 82 };

	//Velocidad/Direccion movimiento: 
	Vector2D <float> dir;

public:
	//Constructor
	Arrow(Vector2D<float>, Point2D<float>, Texture*, Rectangle);

	void render() const;
	bool update(float delta);
};

