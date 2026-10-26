#include "vector2D.h"
#include "rectangle.h"
#include "texture.h"
class Balloon
{
private:
	Vector2D<float> vel; //velocidad
	int color;			//Indice fila textura
	Texture* texture = nullptr; //Puntero a textura
	Rectangle dimension;		
	//constantes 
	//velocidad
	static constexpr float VelEscalar = 100.0f;
	static constexpr float altura = 100.0f;
	static constexpr float anchura = 100.0f;
	//Animacion
	bool explotado = false; //tb para sistema de puntuacion
	//float frameActual = 0.0f; //clave para luego el static cast
	//v.1 con static cast
	//static constexpr float VEL_ANIMACION = 12.0f;

	//v.2
	static constexpr float  TIEMPO_FRAME = 0.08f;
	//int frameActual = 0.0f;
	float timerAnimation = 0.0f; //control de tiempo
	int columnaActual = 0;
	//float VelEscalar = 100;
	
public:
	//Constructor: pos incial ->Rectangle, velocidad, textura, color
	Balloon(Point2D<float>, Vector2D<float>, Texture*, int);
	void render() const;
	bool update(float delta);
	bool hit(const Rectangle& obj);
	bool getterExplotado() const;

};

