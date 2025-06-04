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
	// Test
	Camera camera;
    
    Pos3D pixel1 = camera.get_pixel(0, 0);
    Pos3D pixel2 = camera.get_pixel(500, 500);
    Pos3D pixel3 = camera.get_pixel(999, 999);

    std::cout << "Pixel (0,0) -> " << pixel1.x << ", " << pixel1.y << ", " << pixel1.z << std::endl;
    std::cout << "Pixel (500,500) -> " << pixel2.x << ", " << pixel2.y << ", " << pixel2.z << std::endl;
    std::cout << "Pixel (999,999) -> " << pixel3.x << ", " << pixel3.y << ", " << pixel3.z << std::endl;
	// Test Ende

	cout << "Hello World!" << endl;
	
	Model testmodel;
	testmodel.loadModel("model_cube.stl");
	createPPM();
	
	return 0;
}

