#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "model.h"

using namespace std;

Model::Model(){}; //Constructor (empty atm)

//Load Model file
vector<Facet> Model::loadModel(string file_name) {
	ifstream file(file_name);
	vector<Facet> modelData; 
			
	if (file.is_open()) {
		string line;
		Facet facet;
		int vertexCount = 0; // damit wird gezählt, wie viele "vertex"-Zeilen es waren, weil Dreiecke brauchen genau 3 Seiten (=Zeile)		
		
		while (getline(file, line)) {
		istringstream iss(line);
        string word;
		iss >> word;

			// wenn Zeile mit "facet" losgeht: parset Normale. für "vertex": parset Vertexkoordinaten
			if (word == "facet") {
				iss >> word; 
				iss >> facet.normal.x >> facet.normal.y >> facet.normal.z; // Werte extrahieren
			} else if (word == "vertex") {
				iss >> facet.vertices[vertexCount].x >> facet.vertices[vertexCount].y >> facet.vertices[vertexCount].z;
				vertexCount++;
			}

			// nach drei eingelesen "vertex"-Zeilen/ Seiten wird Facet abgespeichert
			if (vertexCount == 3) {
				modelData.push_back(facet);
				vertexCount = 0; // Reset 
			}
		}

		file.close();

		cout << "Geladene Facetten: " << modelData.size() << endl;
	}
	
	return modelData;
}	
