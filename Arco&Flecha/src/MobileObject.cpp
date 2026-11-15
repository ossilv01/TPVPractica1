#include "MobileObject.h"

bool MobileObject::update(float delta) {
	Vector2D movimiento = speed  * delta;
	dimension.move(movimiento);
	//provisional
	return false;
}
