#pragma once 
#include "vector2D.h"
#include "rectangle.h"
#include "texture.h"
#include "GameObject.h"
class SceneObject : public GameObject
{
private:

protected:
	Point2D <float> position; 
	Texture* texture = nullptr;
	Rectangle dimension;

public: 

	SceneObject(Game* game, Point2D<float> pos, Texture* tex, float width, float height)
		: GameObject(game), position(pos), texture(tex), dimension(pos, width, height) 
	{
	}

	//metodo virtual a overridaear) 
	//false por defecto, quienes lo utilizan le cambiaran el valor
	virtual bool hit(const Rectangle& rect) { return false; };
	

	virtual void save(std::ostream& out) const {
		Point2D<float> pos = dimension.getTopLeftCorner();
		//Guardamos la posicion x e y de cada objeto
		out << pos.getX() << " " << pos.getY() << " ";
	}

	virtual void load(std::istream& in) {
		float x, y;
		in >> x >> y;
		//Actualiza posicion rectangle segun las coordenadas anteriormente guardadas
		dimension = Rectangle(Point2D<float>(x, y), dimension.getWidth(), dimension.getHeight());
	}

};

//EL uso del pragma once sirve porque ocurre lo siguiente
//ERRORES 
// EN bow hereda de SceneObject, pero claro luego bow instancia y crea una flecha
// Ahora arrow hereda de MobileObject, que hereda de SceneObject, 
// asi que en el mismo .cpp abre 2 veces el SceneObject
// Se incluye en un punto SceneObject 2 veces en el mismo archivo, una directa y otra através
// de MobileObject. Se intentno crear la clase 2 veces y se aborto su lectura =errir
// Los errores de clase base sin definir es porque SceneObject dio error
//Bow hereda de los atributos de sceneobject y al fallar al compilar, bow se queda sin ellos
