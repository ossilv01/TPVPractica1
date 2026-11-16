#include "Butterfly.h"
#include "game.h"

Butterfly::Butterfly(Game* game, Point2D<float> pos, Vector2D<float> dir, Texture* textura, int penalizacion) :
	MobileObject(game, pos, dir * velEscalar, textura, anchura, altura), penalizacion(penalizacion)
{
	if (speed.getX() < 0) filaActual = 0; // Izquierda
	else filaActual = 1;		//Derecha
};


void Butterfly::cambiarDireccion(const Vector2D<float>& nuevaDir) {
	float oldX = speed.getX();
	float newX = nuevaDir.getX();
	speed = nuevaDir;

	// Si iba hacia la derecha (o girando a dcha) y ahora va a la izquierda -> Fila 2 (giro dcha->izq)
	if (oldX >= 0 && newX < 0) {
		filaActual = 2;
		columnaActual = 0;
	}
	// Si iba hacia la izquierda (o girando a izq) y ahora va a la derecha -> Fila 3 (giro izq->dcha)
	else if (oldX < 0 && newX >= 0) {
		filaActual = 3;
		columnaActual = 0;
	}

}

void Butterfly::render() const{
	SDL_FRect anim = dimension.to_sdl();
	texture->renderFrame(anim, filaActual, columnaActual);
}


bool Butterfly::update(float delta) {
	MobileObject::update(delta);
	Point2D<float> pos = dimension.getTopLeftCorner();
	float w = dimension.getWidth();
	float h = dimension.getHeight();

	//Mariposa muerta, cayendo
	if (muerta) {
		// Si sale por el borde inferior de la pantalla, se destruye
		if (pos.getY() > game->WINDOW_HEIGHT) {
			return false;
		}
		return true; // Cae con columnaActual = 0
	}

	//Mariposa viva
	Vector2D<float> nuevaVel = speed;
	bool rebote = false;
	//Rebote en los 4 bordes de la ventana
	if (pos.getX() < 0) {
		nuevaVel = Vector2D<float>(std::abs(speed.getX()), speed.getY());
		rebote = true;
	}
	else if (pos.getX() + w > game->WINDOW_WIDTH) {
		nuevaVel = Vector2D<float>(-std::abs(speed.getX()), speed.getY());
		rebote = true;
	}

	if (pos.getY() < 0) {
		nuevaVel = Vector2D<float>(nuevaVel.getX(), std::abs(speed.getY()));
		rebote = true;
	}
	else if (pos.getY() + h > game->WINDOW_HEIGHT) {
		nuevaVel = Vector2D<float>(nuevaVel.getX(), -std::abs(speed.getY()));
		rebote = true;
	}

	if (rebote) {
		cambiarDireccion(nuevaVel);
		dirTimer = 0.0f; // Reiniciamos temporizador aleatorio al rebotar
	}
	else {
		// 3. Cambio aleatorio de dirección cada cierto tiempo
		dirTimer += delta;
		if (dirTimer >= TIEMPO_CAMBIO_DIR) {
			dirTimer = 0.0f;
			// Generamos nueva dirección aleatoria normalizada
			float angle = (static_cast<float>(rand()) / RAND_MAX) * 2.0f * 3.14159f;
			Vector2D<float> randDir(std::cos(angle), std::sin(angle));
			cambiarDireccion(randDir * velEscalar);
		}
	}

	// Animacion
	animTimer += delta;
	if (animTimer >= TIEMPO_FRAME) {
		animTimer = 0.0f;
		columnaActual++;

		if (columnaActual >= NUM_COLUMNAS) {
			columnaActual = 0;

			// Si estaba haciendo animación de giro (filas 2 o 3), pasa al aleteo normal (filas 0 o 1)
			if (filaActual == 2) filaActual = 0;      // Terminó giro a izquierda -> vuela izq
			else if (filaActual == 3) filaActual = 1; // Terminó giro a derecha -> vuela dcha
		}
	}

	return true;


}

bool Butterfly::hit(const Rectangle& rect) {
	if (muerta) return false;

	if (dimension.hasIntersection(rect)) {
		muerta = true;
		columnaActual = 0; // Se queda congelada en la columna 0 de la fila en la que estaba
		speed = Vector2D<float>(0.0f, velCaida); // Solo cae verticalmente hacia abajo

		if (game != nullptr) {
			game->addScore(-penalizacion); // Resta puntos (asegúrate en Game::addScore que score no baje de 0)
		}
		return false; // Devuelve false para NO sumar combo a la flecha
	}
	return false;
}

void Butterfly::save(std::ostream& out) const {
	out << "M ";
	MobileObject::save(out);
	out << filaActual << " " << columnaActual << " " << dirTimer << " " << muerta << "\n";
}

void Butterfly::load(std::istream& in) {
	MobileObject::load(in);
	in >> filaActual >> columnaActual >> dirTimer >> muerta;
}
