#include "light.h"
#include "vector3d.h"

using namespace std;

Light::Light(){
	specular_const = 0.3f;
	diffuse_const = 0.4f;
	ambient_const = 0.1f;
	shininess = 0.1f;
	light_color = {32, 201, 221};
};
