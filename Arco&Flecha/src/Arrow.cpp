#include "Arrow.h"
#include "game.h"
Arrow::Arrow(Vector2D<float> direccion, Point2D<float> posicion, Texture* tex, Game* game) :
	dir(direccion),
	tex(tex),
	tam(posicion, anchura, altura),
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
	float puntaX = tam.getTopLeftCorner().getX() + tam.getHeight();
	if (puntaX>= game->WINDOW_WIDTH) {
		return false;
	}

															//grosor collider punta flecha
	Point2D<float> posPunta(tam.getTopLeftCorner().getX() + tam.getWidth() - anchoPunta, tam.getTopLeftCorner().getY());
	Rectangle colliderPunta(posPunta, anchoPunta, anchoPunta);
	//Colision globos:
	//el tam esta puesto provisionalmente
	//Detecta si ha habido impacto, pero la flecha sigue estando
	game->checkCollision(colliderPunta);
		
	
	return true; 
}
