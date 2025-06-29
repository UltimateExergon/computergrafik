#include "light.h"
#include "vector3d.h"

using namespace std;

Light::Light(){
	specular_const = 0.1f;
	diffuse_const = 0.4f;
	ambient_const = 2.0f;
	shininess = 0.1f;
	
};
