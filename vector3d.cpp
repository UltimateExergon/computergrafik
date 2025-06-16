#include "vector3d.h"

Vector3D::Vector3D(float X, float Y, float Z) : x(X), y(Y), z(Z) {};

void Vector3D::set_vector(Vector3D v){
	x = v.x;
	y = v.y;
	z = v.z;
}

void Vector3D::set_values(float X, float Y, float Z){
	x = X;
	y = Y;
	z = Z;
}
