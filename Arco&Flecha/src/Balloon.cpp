#include "Balloon.h"

Balloon::Balloon(Point2D<float> pos, Vector2D<float>dir, Texture* texture, int color) :
	vel(dir),
	texture(texture),
	color(color),
	dimension(pos, 1536.0f/7.0f, 1792.0f/6.0f)
{
}

void Balloon::render()const {


}


bool Balloon::update(float delta) {

	return false; 
}


bool Balloon::hit(const Rectangle& obj) {

	return false;
}
