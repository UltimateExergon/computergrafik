#include "vector3d.h"

Vector3D::Vector3D(){};

void Vector3D::set_vector(Vector3D v){
	x = v.x;
	y = v.y;
	z = v.z;
}

//Returns dot product of two vectors
float Vector3D::vector_dot(Vector3D v1, Vector3D v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

//Returns cross product of two vectors
Vector3D Vector3D::vector_cross(Vector3D v1, Vector3D v2) {
	Vector3D v3;
	
	v3.x = v1.y * v2.z - v1.z * v2.y;
	v3.y = v1.z * v2.x - v1.x * v2.z;
	v3.z = v1.x * v2.y - v1.y * v2.x;
	
	return v3;
}

//Returns Subtraction of two Vertex
Vector3D Vector3D::vector_substraction(Vector3D v1, Vector3D v2) {
	Vector3D v3;
	
	v3.x = v1.x - v2.x;
	v3.y = v1.y - v2.y;
	v3.z = v1.z - v2.z;
	
	return v3;
}

//Multiply vector with a float
Vector3D Vector3D::vector_times_float(Vector3D v1, float f) {
	Vector3D v2;
	
	v2.x = v1.x * f;
	v2.y = v1.y * f;
	v2.z = v1.z * f;
	
	return v2;
}

//Sum of two vectors
Vector3D Vector3D::vector_addition(Vector3D v1, Vector3D v2) {
	Vector3D v3;
	
	v3.x = v1.x + v2.x;
	v3.y = v1.y + v2.y;
	v3.z = v1.z + v2.z;
	
	return v3;
}
