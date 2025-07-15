#ifndef MODEL_H
#define MODEL_H
#include <string>
#include <vector>
#include "vector3d.h"

using namespace std;

struct Color {
	int r, g, b;
};

struct Facet {
	Vector3D normal; // Flächennormale
	vector<Vector3D> vertices; // Eckpunkte Dreieck
	Color vertexColor = {255, 255, 255}; //Default weiß
};

class Model {
	public:
		vector<Facet> modelData;
	
		Model();
		vector<Facet> loadModel(string file_name);
};

#endif
