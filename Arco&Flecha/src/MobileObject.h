#include "vector2D.h"
#include "SceneObject.h"

class MobileObject : public SceneObject
{
protected:
	Vector2D<float> velocidad; 

public: 
	MobileObject(Game* game, Point2D<float> pos, Vector2D<float> vel, Texture* tex, float width, float height)
		//constructor clase padre + nuevo atributo propio
		: SceneObject(game, pos, tex, width, height), velocidad(vel)
	{ }

	bool update(float delta) override;

};

