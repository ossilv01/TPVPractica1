#include "Arrow.h"
#include "game.h"
Arrow::Arrow(Vector2D<float> direccion, Point2D<float> posicion, Texture* tex, Game* game) :
	dir(direccion),
	tex(tex),
	tam(posicion, 404.0f, 82.0f),
	game(game)
{
};

void Arrow::render()const {
	SDL_FRect anim = tam.to_sdl();
	tex->render(anim);
}

bool Arrow::update(float delta) {
	//velocidad 
	Vector2D desplazamiento = dir * Velocidad * delta;
	tam.move(desplazamiento);

	//Si X supera el ancho de pantalla. La flecha se salio de la pantalla
	//Metodos de rectangle
	if (tam.getTopLeftCorner().getX() >= game->WINDOW_WIDTH) {
		return false;
	}
	return true; 
}
