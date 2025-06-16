#ifndef VECTOR3D_H
#define VECTOR3D_H

class Vector3D {
	public:
		float x, y, z;
		
		Vector3D(float X = 0, float Y = 0, float Z = 0);
		
		void set_vector(Vector3D v);
		void set_values(float X, float Y, float Z);
};

#endif
