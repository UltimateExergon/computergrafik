#include <iostream>
#include <fstream>
#include <vector>
#include "model.h"
#include "camera.h"

using namespace std;

void createPPM(){
	Camera camera;
	
	int maxColors = 255;
	
	vector<int> backgroundColor {0, 0, 0}; //default schwarzer Hintergrund
	
	ofstream ppm_file("output.ppm");
	
	//Add ppm header
	ppm_file << "P3" << endl;
	ppm_file << camera.get_imageWidth() << ' ' << camera.get_imageHeight() << endl;
	ppm_file << maxColors << endl;
	
	//Add model data to ppm
	for (int i = 0; i < camera.get_imageHeight(); i++){
		for (int j = 0; j < camera.get_imageWidth(); j++){
			int r = 255;
			int g = 255;
			int b = 255;
			
			ppm_file << r << ' ' << g << ' ' << b << endl;
		}
	}
	
	ppm_file.close();
}

int main() {
	
	cout << "Hello World!" << endl;
	
	Model testmodel;
	testmodel.loadModel("model_cube.stl");
	createPPM();
	
	return 0;
}
