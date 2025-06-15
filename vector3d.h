#ifndef VECTOR3D_H
#define VECTOR3D_H

class Vector3D {
	public:
		float x, y, z;
		
		Vector3D();
		void set_vector(Vector3D v);
		
		float vector_dot(Vector3D v1, Vector3D v2);
		Vector3D vector_cross(Vector3D v1, Vector3D v2);
		Vector3D vector_substraction(Vector3D v1, Vector3D v2);
		Vector3D vector_times_float(Vector3D v1, float f);
		Vector3D vector_addition(Vector3D v1, Vector3D v2);
};

#endif
