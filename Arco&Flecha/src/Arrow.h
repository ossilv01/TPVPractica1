#include "vector2D.h"
#include "rectangle.h"
#include "texture.h"
#include "MobileObject.h"

//para evitar  dependencia circular
class Game;

class Arrow : public MobileObject
{

private:

	//velocidad escalar:
	static constexpr float Velocidad = 500.0f;
	static constexpr float bonus = 5;
	int globosExplotados = 0; //contador de globos explotados por CADA flecha individualmente
	
public:
	//Constructor
	Arrow(Vector2D<float>, Point2D<float>, Texture*, Game* game);

	static constexpr float anchura = 404.0f / 4;
	static constexpr float altura = 82.0f / 4;
	static constexpr float anchoPunta = 5.0f;

	void render() const override;
	bool update(float delta) override;

	//Cargar de saved.txt
	void save(std::ostream& out) const override;
	void load(std::istream& in) override;
};

