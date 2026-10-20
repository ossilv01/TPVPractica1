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

	//float VelEscalar = 100;
	
public:
	//Constructor: pos incial ->Rectangle, velocidad, textura, color
	Balloon(Point2D<float>, Vector2D<float>, Texture*, int);
	void render() const;
	bool update(float delta);
	bool hit(const Rectangle& obj);

};

