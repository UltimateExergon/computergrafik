#include <iostream>
#include <fstream>
#include <vector>
#include "model.h"
#include "camera.h"

using namespace std;

struct Color {
	int r, g, b;
};

//Returns dot product of two Vertex
float vertex_dot(Vertex v1, Vertex v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

//Returns dot product of Vertex and Pos3D
float vertex_pos_dot(Vertex v1, Pos3D v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

//Returns cross product of two Vertex
Vertex vertex_cross(Vertex v1, Vertex v2) {
	Vertex v3;
	
	v3.x = v1.y * v2.z - v1.z * v2.y;
	v3.y = v1.z * v2.x - v1.x * v2.z;
	v3.z = v1.x * v2.y - v1.y * v2.x;
	
	return v3;
}

//Returns cross product of Vertex and Pos3D
Vertex vertex_pos_cross(Vertex v1, Pos3D v2){
	Vertex v3;
	
	v3.x = v1.y * v2.z - v1.z * v2.y;
	v3.y = v1.z * v2.x - v1.x * v2.z;
	v3.z = v1.x * v2.y - v1.y * v2.x;
	
	return v3;
}

//Returns Subtraction of two Vertex
Vertex vertex_substraction(Vertex v1, Vertex v2) {
	Vertex v3;
	
	v3.x = v1.x - v2.x;
	v3.y = v1.y - v2.y;
	v3.z = v1.z - v2.z;
	
	return v3;
}

//Returns Subtraction of Vertex and Pos3D
Vertex pos_vertex_substraction(Pos3D v1, Vertex v2){
	Vertex v3;
	
	v3.x = v1.x - v2.x;
	v3.y = v1.y - v2.y;
	v3.z = v1.z - v2.z;
	
	return v3;
}


//Creates a PPM output file
void createPPM(Camera cam, Model model){
	vector<Facet> data = model.loadModel("model_cube.stl");
	
	int maxColors = 255;
	
	Color backgroundColor = {0, 0, 0}; //default schwarzer Hintergrund
	Color modelColor = {255, 0, 255}; //default pinkes Model
	
	ofstream ppm_file("output.ppm");
	
	//Add ppm header
	ppm_file << "P3" << endl;
	ppm_file << cam.get_imageWidth() << ' ' << cam.get_imageHeight() << endl;
	ppm_file << maxColors << endl;
	
	//Add model data to ppm
	for (int i = 0; i < cam.get_imageHeight(); i++){
		for (int j = 0; j < cam.get_imageWidth(); j++){
			for (int l = 0; l < int(data.size()); l++) {
				//Möller-Trumbore Intersection Algorithm
				Ray r = cam.get_ray(j, i);
				Facet triangle = data.at(l);
			
				Vertex edge1 = vertex_substraction(triangle.vertices[1], triangle.vertices[0]);
				Vertex edge2 = vertex_substraction(triangle.vertices[2], triangle.vertices[0]);
				
				Vertex c = vertex_pos_cross(edge1, r.direction);
				float det = vertex_dot(edge1, c);
				
				float epsilon = 0.00001f;
				
				if (det < epsilon && det > -epsilon) {
					ppm_file << backgroundColor.r << ' ' << backgroundColor.g << ' ' << backgroundColor.b << endl;
					break; //Ray parallel to triangle
				}
				
				float inv_det = 1.0 / det;
				Vertex s = pos_vertex_substraction(r.origin, triangle.vertices[0]);
				float u = vertex_dot(s, c) * inv_det;
				
				if (u < 0 || u > 1) {
					break;
				}
				
				Vertex q = vertex_cross(s, edge1);
				
				float b = vertex_pos_dot(q, r.direction) * inv_det;
				
				if (b < 0 || u + b > 1) {
					break;
				}
				
				float t = vertex_dot(edge2, q) * inv_det;
				
				if (t > epsilon) {
					ppm_file << modelColor.r << ' ' << modelColor.g << ' ' << modelColor.b << endl;
				}
				else {
					ppm_file << backgroundColor.r << ' ' << backgroundColor.g << ' ' << backgroundColor.b << endl;
				}
			}
		}
	}
	
	ppm_file.close();
	
	cout << "Output File created!" << endl;
}

int main() {
	cout << "Starting Programm" << endl;
	
	Model testmodel;
	Camera camera;
	
	createPPM(camera, testmodel);
	
	system("pause");
	
	return 0;
}

