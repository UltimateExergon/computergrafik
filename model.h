#ifndef MODEL_H
#define MODEL_H
#include <string>
#include <vector>
#include "vector3d.h"

using namespace std;

struct Color {
	uint8_t b, g, r; //Swapped r and b, because apparently for some reason the color values are in this order in the BMP file
};

struct Facet {
	Vector3D normal; // Flächennormale
	vector<Vector3D> vertices; // Eckpunkte Dreieck
	Color vertexColor = {255, 255, 255}; //Default weiß
	vector<Vector2D> uv;
};

class Model {
	public:
		vector<Facet> modelData;
	
		Model();
		vector<Facet> loadModel(string file_name, string uvFile_name);
};

#endif
