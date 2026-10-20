#include "Arrow.h"
#include "game.h"
Arrow::Arrow(Vector2D<float> direccion, Point2D<float> posicion, Texture* tex, Game* game) :
	dir(direccion),
	tex(tex),
	tam(posicion, 404.0f/9, 82.0f/9),
	game(game)
{
};

//404.0f, 82.0f
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
	float puntaX = tam.getTopLeftCorner().getX() + tam.getWidth();
	if (puntaX>= game->WINDOW_WIDTH) {
		return false;
	}

	//Colision globos:
	if (game->checkCollision(tam)) {

	}
	return true; 
}
