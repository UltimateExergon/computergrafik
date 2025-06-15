#ifndef MODEL_H
#define MODEL_H
#include <string>
#include <vector>
#include "vector3d.h"

using namespace std;

struct Facet {
	Vector3D normal; // Flächennormale
	Vector3D vertices[3]; // Eckpunkte Dreieck
};

class Model {
	public:
		Model(); //Constructor (empty atm)
		vector<Facet> loadModel(string file_name);
	private:
		// speichert Liste von Facets in einem Vektor
		vector<Facet> modelData;
};

#endif
