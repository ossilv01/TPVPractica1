#include "vector2D.h"
#include "rectangle.h"
#include "texture.h"
class Balloon
{
private:
	Vector2D<float> vel; 
	int color;
	Texture* texture = nullptr;
	Rectangle pos;
	//Necesitaria un point2D para la esquina ? 
	//dimensiones: 
	Point2D <int> esquina = 0; 
	Rectangle dim{esquina, 1536/7, 1792/6};
	//Posciones y dimensiones (Rectangle) Alto x ancho
	//1536/7, 1792/6
	
public:
	//Constructor por ahora
	Balloon(Vector2D<float>, Texture*, int);
	void render() const;
	bool update(float delta);
	bool hit(const Rectangle& obj);

};

