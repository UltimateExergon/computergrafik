#ifndef VECTOR3D_H
#define VECTOR3D_H
#include "stdint.h"

//Putting this here so we don't have to include model.h everywhere
struct Color {
	uint8_t b, g, r; //Swapped r and b, because apparently for some reason the color values are in this order in the BMP file
};

//Just putting this here cause why make a new file just for this one struct
struct Vector2D{
	float x, y;
};

class Vector3D {
	public:
		float x, y, z;
		
		Vector3D(float X = 0, float Y = 0, float Z = 0);
		
		void set_vector(Vector3D v);
		void set_values(float X, float Y, float Z);
};

float vector_dot(Vector3D v1, Vector3D v2);
Vector3D vector_addition(Vector3D v1, Vector3D v2);
Vector3D vector_subtraction(Vector3D v1, Vector3D v2);  
Vector3D vector_multiplication(Vector3D v1, Vector3D v2);
Vector3D vector_times_float(Vector3D v1, float f);
Vector3D vector_cross(Vector3D v1, Vector3D v2);
Vector3D normalize(Vector3D v);
Vector3D barycentric_to_cartesian(Vector3D b, Vector3D v1, Vector3D v2, Vector3D v3);

#endif
