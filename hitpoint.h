#ifndef HITPOINT_H
#define HITPOINT_H
#include "model.h"
#include <limits>

using namespace std;

struct Pos3D {
	float x, y, z;
};

class Hitpoint{
	public:
		Pos3D position;
		float distance = numeric_limits<float>::max();
		const Model* model = nullptr;
		
		Hitpoint();
};

#endif
