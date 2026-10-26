#include "Balloon.h"
Balloon::Balloon(Point2D<float> pos, Vector2D<float>dir, Texture* texture, int color) :
	vel(dir),
	texture(texture),
	color(color),
	dimension(pos, anchura, altura)
{
}

void Balloon::render()const {
	SDL_FRect anim = dimension.to_sdl();
	int fil = color;

	//V.1.0 static cas
	
	/*int col = 0;
	if (explotado) {
		col = static_cast<int>(frameActual);
		if (col < 1) col = 1;
		if (col > 5) col = 5;
	}

	texture->renderFrame(anim, fil, col);*/
	texture->renderFrame(anim, fil, columnaActual);

}


bool Balloon::update(float delta) {

	if (!explotado) {
		Vector2D movimiento = vel * (VelEscalar * delta);
		dimension.move(movimiento);

		//Esquina superior del rectangulo
		float posY = dimension.getTopLeftCorner().getY();
		//Y altura del globo
		float alto = dimension.getHeight();

		//Si ambas son menores que 0, ya no estan en pantalla
		if ((posY + alto) < 0) return false;
	}
	else {
		//V.2
		timerAnimation += delta;
		if (timerAnimation >= TIEMPO_FRAME) {
			columnaActual++;
			timerAnimation = 0.0f;

			if (columnaActual > 5) {
					return false;
			}
		}

		//V.1
		//frameActual += VEL_ANIMACION * delta;
		//if (frameActual >= 6.0f) {
		//	return false; // se acabo la animación
		//}
	}
	return true; 
}


bool Balloon::hit(const Rectangle& obj) {
	
	if (explotado) return false;
	
	if (dimension.hasIntersection(obj)) {
		explotado = true;
		return true;
	}
	return false;
}

bool Balloon::getterExplotado() const {
	return explotado;
}
