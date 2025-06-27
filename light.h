#ifndef LIGHT_H
#define LIGHT_H
#include "vector3d.h"

class Light {
	public:
		Vector3D light_pos;
		float specular_const;
		float diffuse_const;
		float ambient_const;
		float shininess;
		
		Light();
};


#endif
