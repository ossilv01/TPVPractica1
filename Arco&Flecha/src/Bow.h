#include "SDL3/SDL.h"
#include "texture.h"
#include "vector2D.h"
#include "rectangle.h"
#include "SceneObject.h"

class Game;
class Bow : public SceneObject
{
private:
	//dimensiones
	static constexpr float altura = 100.0f;
	static constexpr float anchura = 100.0f;
	
	//heredamos: game, texture, position y dimension (rectangle)

	////puntero a textura
	//Texture* tex = nullptr; 
	////puntero a game
	//Game* game = nullptr;
	////Dimensiones 
	//Rectangle tam;

	// Atributos exclusivos
	//Armado o no
	bool armed;
	//Direccion movimienot
	Vector2D<float> dir;
	//numero flechas
	int numArrows = 5; 
	//Velocidad escalar
	static constexpr float velocidadEscalar = 300.0f;


public:

	//Constructor:
	Bow(Vector2D<float>, Point2D<float>, int, Texture*, Game*);
	//Metodos
	void handleEvent(const SDL_Event& event);

	//Metodos sobreescribir
	void render() const override;
	bool update(float delta) override;

	//El equivalente a las vidas
	int getFlechas() const;
	//Metodos para disparo
	bool canShoot() const;
	void Shoot();

	//Cargar de saved.txt
	void save(std::ostream& out) const;
	void load(std::istream& in);

	//getter de Point2D
	Point2D <float> getPosition() const;
	int setFlechas(int flechas) { return numArrows = flechas; };

};

