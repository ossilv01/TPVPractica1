#include "Arrow.h"
#include "game.h"
Arrow::Arrow(Vector2D<float> direccion, Point2D<float> posicion, Texture* tex, Game* game) :
	//		: SceneObject(game, pos, tex, width, height), velocidad(vel)
	MobileObject(game, posicion, direccion * Velocidad, tex, anchura, altura)
	/*dir(direccion),
	tex(tex),
	tam(posicion, anchura, altura),
	game(game)*/
{
};

//404.0f, 82.0f
//dimension = Rectangle 
// speed = Vector2D <float> Direccion 

void Arrow::render()const {
	SDL_FRect anim = dimension.to_sdl();
	texture->render(anim);
}

bool Arrow::update(float delta) {
	//velocidad 
	MobileObject::update(delta);
	//Vector2D desplazamiento = speed * Velocidad * delta;
	//dimension.move(desplazamiento);

	//Si X supera el ancho de pantalla. La flecha se salio de la pantalla
	//Metodos de rectangle
	float puntaX = dimension.getTopLeftCorner().getX() + dimension.getHeight();
	if (puntaX>= game->WINDOW_WIDTH) {
		//Contador de flechas activas para game
		if (game != nullptr) {
			game->removeArrow(); //Sera 0 cuando la flecha supere el borde derecho
		 }
		return false;
	}
															//grosor collider punta flecha
	Point2D<float> posPunta(dimension.getTopLeftCorner().getX() + dimension.getWidth() - anchoPunta, dimension.getTopLeftCorner().getY());
	Rectangle colliderPunta(posPunta, anchoPunta, anchoPunta);
	//Colision globos:
	//Detecta si ha habido impacto, pero la flecha sigue estando
	if (game->checkCollision(colliderPunta)) {
		if (globosExplotados > 0) {
			//A partir del segundo globo seguido se aplica el bonus
			game->addScore(globosExplotados * bonus);
		}
		globosExplotados++;
	}
	return true; 
}

void Arrow::save(std::ostream& out) const {
	out << "A ";
	MobileObject::save(out);
	out << globosExplotados << "\n";
}

void Arrow::load(std::istream& in) {
	MobileObject::load(in);
	in >> globosExplotados;
}
