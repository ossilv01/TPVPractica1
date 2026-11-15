#include "vector2D.h"
#include "rectangle.h"
#include "texture.h"
#include "GameObject.h"
class SceneObject : public GameObject
{
private:

protected:
	Point2D <float> position; 
	Texture* texture = nullptr;
	Rectangle dimension;

public: 

	SceneObject(Game* game, Point2D<float> pos, Texture* tex, float width, float height)
		: GameObject(game), position(pos), texture(tex), dimension(pos, width, height) 
	{
	}

			//metodo virtual a overridaear) 
	virtual bool hit(const Rectangle& rect);
};

