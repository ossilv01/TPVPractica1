#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>

/**
 * Vector bidimensional genérico.
 */
template<std::floating_point T = float>
class Vector2D
{
	T x, y;

public:
	Vector2D(T x, T y) : x(x), y(y) { }
	Vector2D() : Vector2D(0, 0) { }

	// Coordenadas del vector
	T getX() const { return x; }
	T getY() const { return y; }

	//Obtener longitud del vector, modulo (raiz x^2 + y^2)
	T lenght() const { return sqrt(x * x + y * y); }

	// Operadores
	Vector2D operator+(const Vector2D& otro) const {
		return {x + otro.x, y + otro.y};
	}

	//Operador +=
	Vector2D operator+=(const Vector2D& otro) {
		x += otro.x;
		y += otro.y;
		return *this; //Devolvemos objeto modificado
	}

	//operador -
	Vector2D operator-(const Vector2D& otro)const {
		return { x - otro.x, y - otro.y };
	}

	//Operador * (producto por un escalar)
	Vector2D operator* (T escalar)const {
		return { x * escalar, y * escalar };
	}

	//Producto escalar
	//Devuelve un numero tipo T (por definicion de las mates) 
	T operator*(const Vector2D& otro) {
		return { (x * otro.x) + (y * otro.y) };
	}


	// TODO: completar

	// Operadores de entrada/salida
	friend std::ostream& operator<<(std::ostream& out, const Vector2D& v) {
		return out << '{' << v.x << ", " << v.y << '}';
	}
};

// TODO: definir alias Point2D<T> (float o double) 
template<std::floating_point T = float>
using Point2D = Vector2D<T>;

#endif // VECTOR2D_H
