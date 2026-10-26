#include "Bow.h"
#include "game.h"
#include "Arrow.h"

Bow::Bow(Vector2D<float> direccion, Point2D<float> posicion, int arrows, Texture* textura, Game* game) :
	dir(direccion),
	flechas(arrows),
	tex(textura),
	tam(posicion, anchura, altura),
	game(game),
	armed(false)
{
}
//804.0f / 2.0f, 381.0f

void Bow::handleEvent(const SDL_Event& event) {
	if (event.type == SDL_EVENT_KEY_DOWN) { 	//Presiona tecla hacia abajo
		switch (event.key.key) {
		case SDLK_W: case SDLK_UP:
			dir = Vector2D<float>(0, -1); //Hacia arriba
			break;
		case SDLK_S: case SDLK_DOWN:
			dir = Vector2D<float>(0, 1); //Hacia abajo
			break;

		case SDLK_A: case SDLK_LEFT:	
			if (flechas > 0 && !armed) {
				armed = true;
			}
			break;
		case SDLK_D: case SDLK_RIGHT:
			if (canShoot()) {
				Point2D<float> arcoPos = getPosition();   //Punto referencia de flecha respecto a Arco (esquina)
				//armed = false;
				float a = tam.getHeight();		//100.0f =altura arco 
				float b = Arrow::altura; //altura flecha
				//altura flecha
				float flechaY = arcoPos.getY() + (a/2.0f) - (b / 2.0f);  //Centrar flecha (Alto objeto Grande + Alto objeto) /2
				Point2D<float> posCentro(arcoPos.getX(), flechaY); 		//Nuevo punto centrado:
				Arrow* newFlecha = new Arrow(Vector2D<float>(1, 0), posCentro,game->getTexture(Game::ARROW),game); //instanciamos flecha
				game->addArrow(newFlecha);
				Shoot();
			}
			break;
		}
	}
	else if (event.type == SDL_EVENT_KEY_UP) { 	//Deja de presionar tecla
		switch (event.key.key) {
		case SDLK_W: case SDLK_UP: 			//Para que no se mueva infinitamente. Paramos subir
			if (dir.getY() < 0) dir = Vector2D<float>(0, 0);
			break;
		case SDLK_S: case SDLK_DOWN:			//Paramos al bajar
			if (dir.getY() > 0) dir = Vector2D<float>(0, 0);
			break;
		}
	}
}

void Bow::render()const {
	//Del RECTANGLE definido para tema Colliders y movimiento, lo transformamos en SDL_FRect
	SDL_FRect anim = tam.to_sdl();

	//Cambio entre columna de frame: Armado y desarmado
	int columna;
	if (armed) columna = 0;
	else columna = 1;
	int fila = 0;

	//Mediante el puntero a texture, accedemos al metodo render apropiado
	tex->renderFrame(anim,fila,columna);
}

// nuevo parametro para obtener Window_Height (se declara en game.cpp)
bool Bow::update(float delta) {

	// v= direccion * velocidad * tiempo;
	Vector2D<float> desplazamiento = dir * (velocidadEscalar * delta);

	//Posicion actual esquina
	Point2D<float> esquina = tam.getTopLeftCorner();

	//tamaño altura arco
	float altoArco = tam.getHeight();
	//Actuación preventiva asumiendo Y tras el movimiento
	float nextY = esquina.getY() + desplazamiento.getY();

	//Limite superior
	if (nextY < 0) {
		desplazamiento = Vector2D<float>(desplazamiento.getX(), -esquina.getY());
	}
	//Limite inferior
	else if (nextY + altoArco > game->WINDOW_HEIGHT){
		float spaceLeft= game->WINDOW_HEIGHT - (esquina.getY() + altoArco);
		desplazamiento = Vector2D<float>(desplazamiento.getX(), spaceLeft);
	}

	//Metodo de Rectangle para tb acutalizar su posicion;
	tam.move(desplazamiento);

	//Nunca se """destruye""" el arco
	return true;
}

bool Bow::canShoot() const {
	return armed;
}

void Bow::Shoot() {
	armed = false;
	if (flechas > 0) flechas--;
}

int Bow::getFlechas() const {
	return flechas;
}

Point2D<float> Bow::getPosition() const {
	return tam.getTopLeftCorner();
}
