#include "Balloon.h"
Balloon::Balloon(Point2D<float> pos, Vector2D<float>dir, Texture* texture, int color) :
	vel(dir),
	texture(texture),
	color(color),
	dimension(pos, 1536.0f / 7.0f, 1792.0f / 6.0f)
{
}

void Balloon::render()const {
	SDL_FRect anim = dimension.to_sdl();
	int fil = color;
	int col = 0;
	texture->renderFrame(anim, fil, col);

}


bool Balloon::update(float delta) {
	Vector2D movimiento = vel * (VelEscalar * delta);
	dimension.move(movimiento);

	//Esquina superior del rectangulo
	float posY = dimension.getTopLeftCorner().getY();
	//Y altura del globo
	float alto = dimension.getHeight();

	//Si ambas son menores que 0, ya no estan en pantalla
	if ((posY + alto) < 0) return false;

	return true; 
}


bool Balloon::hit(const Rectangle& obj) {
	return dimension.hasIntersection(obj);
}
