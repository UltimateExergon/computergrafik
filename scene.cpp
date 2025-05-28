#include <iostream>
#include <fstream>
#include "model.h"

using namespace std;

void createPPM(){
	int imageHeight = 200;
	int imageWidth = 200;
	int maxColors = 255;
	
	
	ofstream ppm_file("output.ppm");
	
	//Add ppm header
	ppm_file << "P3" << endl;
	ppm_file << imageWidth << ' ' << imageHeight << endl;
	ppm_file << maxColors << endl;
	
	//Add model data to ppm
	for (int i = 0; i < imageHeight; i++){
		for (int j = 0; j < imageWidth; j++){
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
