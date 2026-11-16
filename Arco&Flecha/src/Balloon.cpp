#include "Balloon.h"
#include "game.h"

Balloon::Balloon(Point2D<float> pos, Vector2D<float>dir, Texture* texture, int color, int puntos, Game* game) :
	/*vel(dir),
	texture(texture),*/
	//		: SceneObject(game, pos, tex, width, height), velocidad(vel)
								//
	MobileObject(game, pos, dir, texture, anchura, altura),
	color(color),
	//dimension(pos, anchura, altura),
	puntos(puntos)
	//game(game)
{
}

void Balloon::render()const {
	SDL_FRect anim = dimension.to_sdl();
	int fil = color;
	texture->renderFrame(anim, fil, columnaActual);

}


bool Balloon::update(float delta) {

	if (!explotado) {
		/*Vector2D movimiento = speed * (VelEscalar * delta);
		dimension.move(movimiento);*/
		//Esta logica de movimiento esta metida ahora en el update de MobileObject
		MobileObject::update(delta);

		//Esquina superior del rectangulo
		float posY = dimension.getTopLeftCorner().getY();
		//Y altura del globo
		float alto = dimension.getHeight();

		//Si ambas son menores que 0, ya no estan en pantalla
		if ((posY + alto) < 0) return false;
	}
	else {
		//animacion
		timerAnimation += delta;
		if (timerAnimation >= TIEMPO_FRAME) {
			columnaActual++;
			timerAnimation = 0.0f;

			if (columnaActual > 5) {
					return false;
			}
		}
	}
	return true; 
}


bool Balloon::hit(const Rectangle& obj) {
	
	if (explotado) return false;
	
	if (dimension.hasIntersection(obj)) {
		explotado = true;
		//añadimos puntos al score
		if (game != nullptr) {
			game->addScore(puntos);
		}
		return true;
	}
	return false;
}

