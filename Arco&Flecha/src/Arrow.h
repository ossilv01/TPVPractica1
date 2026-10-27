#include "vector2D.h"
#include "rectangle.h"
#include "texture.h"

//para evitar  dependencia circular
class Game;

class Arrow
{

private:
	//dimensiones
	//404.0f/9, 82.0f/9


	//Puntero textura
	Texture* tex = nullptr; 

	//Puntero a game para colisiones/Aprovechamos para variables publicas GAME
	Game* game = nullptr;

	//Dimensiones:
	Rectangle tam;

	//Velocidad/Direccion movimiento: 
	Vector2D <float> dir;

	//velocidad escalar:
	float Velocidad = 500.0f;

	//grosor collider punta flecha


public:
	//Constructor
	Arrow(Vector2D<float>, Point2D<float>, Texture*, Game* game);

	static constexpr float anchura = 404.0f / 4;
	static constexpr float altura = 82.0f / 4;
	static constexpr float anchoPunta = 5.0f;

	void render() const;
	bool update(float delta);
};

