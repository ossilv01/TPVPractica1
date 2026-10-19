#include "Arrow.h"

Arrow::Arrow(Vector2D<float> direccion, Point2D<float> posicion, Texture* tex, Rectangle dimension) :
	dir(direccion),
	pos(posicion),
	tex(tex),
	tam(dimension)
{
};

void Arrow::render() {

}

bool Arrow::update(float delta) {


	return false; 
}
