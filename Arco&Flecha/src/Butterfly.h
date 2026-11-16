#pragma once
#include "MobileObject.h"
#include "texture.h"
class Game;
class Butterfly : public MobileObject
{
private:

	static constexpr float altura = 60.0f;
	static constexpr float anchura = 60.0f;
	static constexpr float velEscalar = 120.0f;
	static constexpr float velCaida = 250.0f;

	static constexpr float TIEMPO_FRAME = 0.06f;
	static constexpr int NUM_COLUMNAS = 10;
	float animTimer = 0.0f;
	int filaActual = 1;
	int columnaActual = 0;

	static constexpr float TIEMPO_CAMBIO_DIR = 2.5f;//en segundos
	float dirTimer = 0.0f;

	bool muerta = false;
	int penalizacion = 5;

	void cambiarDireccion(const Vector2D<float>& nuevaDir);

public:
	//: SceneObject(game, pos, tex, width, height), speed(vel)
	//Constructor (por ahora con cosas base)
	Butterfly(Game* game, Point2D<float> pos, Vector2D<float> dir, Texture* textura, int penalizacion);
	//Metodos de padres para override
	void render() const override;
	bool update(float delta) override;
	bool hit(const Rectangle&) override;

	//Metodos propios logica
	bool checkBounds();
	void updateAnimacion(float delta);
	//Cargar de saved.txt
	void save(std::ostream& out) const override;
	void load(std::istream& in) override;
};

