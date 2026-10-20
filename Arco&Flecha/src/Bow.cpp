#include "Bow.h"

Bow::Bow(Vector2D<float> direccion , Point2D<float> posicion, int arrows, Texture* textura):
	dir(direccion),
	flechas(arrows),
	tex(textura),
	tam(posicion,804.0f / 2.0f, 381.0f)
{
}

void Bow::handleEvent(const SDL_Event& event) {
	//Presiona tecla hacia abajo
	if (event.type == SDL_EVENT_KEY_DOWN) {
		switch (event.key.key) {
		case SDLK_W: case SDLK_UP:
			dir = Vector2D<float>(0, -1); //Hacia arriba
			break;
		case SDLK_S: case SDLK_DOWN:
			dir = Vector2D<float>(0, 1); //Hacia abajo
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
	//Deja de presionar tecla
	else if (event.type == SDL_EVENT_KEY_UP) {
		switch (event.key.key) {
			//Para que no se mueva infinitamente
			//Paramos subir
		case SDLK_W: case SDLK_UP:
			if (dir.getY() < 0) dir = Vector2D<float>(0, 0);
			break;
			//Paramos al bajar
		case SDLK_S: case SDLK_DOWN:
			if (dir.getY() > 0) dir = Vector2D<float>(0, 0);
			break;

		default:
			break;
		}
	}
}

void Bow::render()const {
	//Del RECTANGLE definido para tema Colliders y movimiento, lo transformamos en SDL_FRect
	SDL_FRect anim = tam.to_sdl();

	//Cambio entre columna de frame: Armado y desarmado
	int columna;
	if (armed) columna = 1;
	else columna = 0;
	int fila = 0;

	//Mediante el puntero a texture, accedemos al metodo render apropiado
	tex->renderFrame(anim,fila,columna);
}

bool Bow::update(float delta) {

	// v= direccion * velocidad * tiempo;
	Vector2D<float> desplazamiento = dir * (velocidadEscalar * delta);
	//Metodo de Rectangle para tb acutalizar su posicion;
	tam.move(desplazamiento);

	//Nunca se """destruye""" el arco
	return true;
}

int Bow::getFlechas() const {
	return flechas;
}
