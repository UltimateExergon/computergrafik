#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include "model.h"

using namespace std;

Model::Model(){}; //Constructor (empty atm)

//Load Model file
vector<Facet> Model::loadModel(string file_name, string uvFile_name) {
	ifstream file(file_name);
	Facet facet;
			
	if (file.is_open()) {
		string line;
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
				Vector3D vertex;
				iss >> vertex.x >> vertex.y >> vertex.z;
				facet.vertices.push_back(vertex);
				vertexCount++;
			}

			// nach drei eingelesen "vertex"-Zeilen/ Seiten wird Facet abgespeichert
			if (vertexCount == 3) {
				modelData.push_back(facet);
				facet.vertices.clear();
				vertexCount = 0; // Reset 
			}
		}

		file.close();
	}
	
	//assign uv coordinates to facets
	ifstream c_file(uvFile_name);
	
	vector<Vector2D> unsortedCoords;

	if (c_file.is_open()){
		string c_line;

		while (getline(c_file, c_line)){
			Vector2D uv_coords;
			istringstream issc(c_line);
			
			issc >> uv_coords.x >> uv_coords.y;
			
			unsortedCoords.push_back(uv_coords);
		}
		
		c_file.close();

	}
	
	for (int i = 0; i < int(modelData.size()); i++){
		if (i % 2 == 0){
			modelData[i].uv.push_back(unsortedCoords[0]);
			modelData[i].uv.push_back(unsortedCoords[1]);
			modelData[i].uv.push_back(unsortedCoords[2]);
		}
		else{
			modelData[i].uv.push_back(unsortedCoords[3]);
			modelData[i].uv.push_back(unsortedCoords[4]);
			modelData[i].uv.push_back(unsortedCoords[5]);
		}
	}
	
	cout << "Geladene Facetten: " << modelData.size() << endl;
	
	return modelData;
}	
