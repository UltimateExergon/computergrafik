#ifndef MODEL_H
#define MODEL_H
#include <string>
#include <vector>
#include "vector3d.h"

using namespace std;

struct Facet {
	Vector3D normal; // Flächennormale
	vector<Vector3D> vertices; // Eckpunkte Dreieck
	vector<Vector2D> uv;
};

class Model {
	public:
		vector<Facet> modelData;
	
		Model();
		vector<Facet> loadModel(string file_name, string uvFile_name);
};

#endif
