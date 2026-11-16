#include "vector2D.h"
#include "rectangle.h"
#include "texture.h"
#include "MobileObject.h"

//para evitar  dependencia circular
class Game;

class Arrow : public MobileObject
{

private:
	//dimensiones
	//404.0f/9, 82.0f/9

	//		: SceneObject(game, pos, tex, width, height), velocidad(vel)

	//Puntero textura
	//Texture* tex = nullptr; 

	////Puntero a game para colisiones/Aprovechamos para variables publicas GAME
	//Game* game = nullptr;

	////Dimensiones:
	//Rectangle tam;

	////Velocidad/Direccion movimiento: 
	//Vector2D <float> dir;

	//velocidad escalar:
	static constexpr float Velocidad = 500.0f;
	static constexpr float bonus = 5;
	int globosExplotados = 0; //contador de globos explotados por CADA flecha individualmente
	
public:
	//Constructor
	Arrow(Vector2D<float>, Point2D<float>, Texture*, Game* game);

	static constexpr float anchura = 404.0f / 4;
	static constexpr float altura = 82.0f / 4;
	static constexpr float anchoPunta = 5.0f;

	void render() const override;
	bool update(float delta) override;
};

