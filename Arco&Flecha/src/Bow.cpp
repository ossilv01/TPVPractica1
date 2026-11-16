#include "Bow.h"
#include "game.h"
#include "Arrow.h"

Bow::Bow(Vector2D<float> direccion, Point2D<float> posicion, int arrows, Texture* textura, Game* game) :
	SceneObject(game, posicion, textura, anchura, altura),
	armed(false),
	dir(direccion),
	numArrows(arrows)
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
			if (numArrows > 0 && !armed) {
				armed = true;
			}
			break;
		case SDLK_D: case SDLK_RIGHT:
			if (canShoot()) {
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
//"dimension" y "texture", nombres del Rectangle y Texture* del padre "SceneObject"

void Bow::render()const {
	//Del RECTANGLE definido para tema Colliders y movimiento, lo transformamos en SDL_FRect
	SDL_FRect anim = dimension.to_sdl();

	//Cambio entre columna de frame: Armado y desarmado
	int columna;
	if (armed) columna = 0;
	else columna = 1;
	int fila = 0;

	//Mediante el puntero a texture, accedemos al metodo render apropiado
	texture->renderFrame(anim,fila,columna);
}

// nuevo parametro para obtener Window_Height (se declara en game.cpp)
bool Bow::update(float delta) {

	// v= direccion * velocidad * tiempo;
	Vector2D<float> desplazamiento = dir * (velocidadEscalar * delta);

	//Posicion actual esquina
	Point2D<float> esquina = dimension.getTopLeftCorner();

	//tamaño altura arco
	float altoArco = dimension.getHeight();
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
	dimension.move(desplazamiento);

	//Nunca se """destruye""" el arco
	return true;
}

bool Bow::canShoot() const {
	return armed;
}

void Bow::Shoot() {
	//Cambio de sprite y resta de flechas
	armed = false;
	if (numArrows > 0) numArrows--;

	//Creacion y disparo de flecha
	Point2D<float> arcoPos = getPosition();   //Punto referencia de flecha respecto a Arco (esquina)
	//armed = false;
	float a = dimension.getHeight();		//100.0f =altura arco 
	float b = Arrow::altura; //altura flecha
	//altura flecha
	float flechaY = arcoPos.getY() + (a / 2.0f) - (b / 2.0f);  //Centrar flecha (Alto objeto Grande + Alto objeto) /2
	Point2D<float> posCentro(arcoPos.getX(), flechaY); 		//Nuevo punto centrado:
	Arrow* newFlecha = new Arrow(Vector2D<float>(1, 0), posCentro, game->getTexture(Game::ARROW), game); //instanciamos flecha
	game->addArrow(newFlecha);
}

int Bow::getFlechas() const {
	return numArrows;
}

Point2D<float> Bow::getPosition() const {
	return dimension.getTopLeftCorner();
}

void Bow::save(std::ostream& out) const {
	out << "W ";
	SceneObject::save(out);
	out << numArrows << " " << armed << "\n";
}

void Bow::load(std::istream& in) {
	SceneObject::load(in);
	in >> numArrows >> armed;
}
