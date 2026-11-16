#include "vector2D.h"
#include "rectangle.h"
#include "texture.h"
#include "MobileObject.h"
class Game;

class Balloon : public MobileObject
{
private:

	//atributos propios
	int color;			//Indice fila textura
	int puntos;

	//constantes 
	//velocidad
	static constexpr float VelEscalar = 100.0f;
	static constexpr float altura = 100.0f;
	static constexpr float anchura = 100.0f;
	//Animacion
	bool explotado = false; //tb para sistema de puntuacion
	//v.2
	static constexpr float  TIEMPO_FRAME = 0.08f; //segundos 
	float timerAnimation = 0.0f; //control de tiempo
	int columnaActual = 0;
	
public:
	//Constructor: pos incial ->Rectangle, velocidad, textura, color, puntos
	Balloon(Point2D<float>, Vector2D<float>, Texture*, int, int, Game* game);
	void render() const override;
	bool update(float delta) override;
	bool hit(const Rectangle& obj) override;
	//bool getterExplotado() const;
	
	//Cargar de saved.txt
	void save(std::ostream& out) const override;
	void load(std::istream& in) override;

};

