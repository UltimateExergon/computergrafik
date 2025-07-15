#ifndef VECTOR3D_H
#define VECTOR3D_H

struct Vector2D {
	int x, y;
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

float length2D(Vector2D v);
Vector2D vector_subtraction2D(Vector2D v1, Vector2D v2);

#endif
