#ifndef MODEL_H
#define MODEL_H
#include <string>
#include <vector>

using namespace std;

// Vertex Koordinaten
struct Vertex {
	float x, y, z;
};

struct Facet {
	Vertex normal; // Flächennormale
	Vertex vertices[3]; // Eckpunkte Dreieck
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
