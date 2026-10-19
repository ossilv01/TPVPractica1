#include "vector2D.h"
#include "rectangle.h"
#include "texture.h"
class Balloon
{
private:
	Vector2D<float> vel; 
	int color;
	Texture* texture = nullptr;

	//Posciones y dimensiones (Rectangle)
	//----
public:
	//Constructor por ahora
	Balloon(Vector2D<float>, Texture*, int);
	void render();
	bool update(float delta);
	bool hit(const Rectangle& obj);

};

