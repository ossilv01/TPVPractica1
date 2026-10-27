#include "vector2D.h"
#include "rectangle.h"
class SceneObject
{
private: 
	Point2D <float> pos; 
	Rectangle a; 

public: 
	bool hit(const Rectangle&);
};

