#include "Bow.h"

Bow::Bow(Vector2D<float> direccion , Point2D<float> posicion, int arrows, Texture* textura, Rectangle tamano):
	move(direccion),
	pos(posicion),
	flechas(arrows),
	tex(textura),
	tam(tamano)
{

}

void Bow::handleEvent(const SDL_Event& event) {
	if (event.type == SDL_EVENT_KEY_DOWN) {
		switch (event.key.key) {
		case SDLK_W: case SDLK_UP:
			move = Vector2D<float>(0, -1);
			break;
		case SDLK_S: case SDLK_DOWN:
			move = Vector2D<float>(0, 1);
			break;

		case SDLK_A: case SDLK_LEFT:
			flechas--;
			armed = true;
			break;

		case SDLK_D: case SDLK_RIGHT:
			armed = false;
			break;

		default:
			break;
		}
	}
}

void Bow::render()const {
	//Del RECTANGLE definido para tema Colliders y movimiento, lo transformamos en SDL_FRect
	SDL_FRect anim = tam.to_sdl();
	//Mediante el puntero a texture, accedemos al metodo render apropiado
	tex->render(anim);
}

bool Bow::update(float delta) {
	
	return false;
}

int Bow::getFlechas() const {
	return flechas;
}
