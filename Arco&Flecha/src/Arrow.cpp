#include "Arrow.h"

Arrow::Arrow(Vector2D<float> direccion, Point2D<float> posicion, Texture* tex, Rectangle dimension) :
	dir(direccion),
	pos(posicion),
	tex(tex),
	tam(dimension)
{
};

void Arrow::render()const {
	SDL_FRect anim = tam.to_sdl();
	tex->render(anim);
}

bool Arrow::update(float delta) {


	return false; 
}
