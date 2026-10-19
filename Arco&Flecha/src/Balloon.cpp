#include "Balloon.h"

Balloon::Balloon(Vector2D<float>dir, Texture* texture, int color) :
	vel(dir),
	texture(texture),
	color(color)
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
