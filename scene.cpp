#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <math.h>
#include "model.h"
#include "camera.h"
#include "vector3d.h"

using namespace std;

struct Color {
	int r, g, b;
};

Hitpoint intersection(Facet triangle, Ray r){
	//Möller-Trumbore Intersection Algorithm
	Hitpoint hit;
	Vector3D edge1;
	Vector3D edge2;
	Vector3D c;
	Vector3D s;
	Vector3D q;
	
	edge1.set_vector(edge1.vector_substraction(triangle.vertices[1], triangle.vertices[0]));
	edge2.set_vector(edge2.vector_substraction(triangle.vertices[2], triangle.vertices[0]));
				
	c.set_vector(c.vector_cross(edge2, r.direction));
	
	float det = c.vector_dot(edge1, c);
			
	float epsilon = 0.00001f;
				
	if (fabsf(det) < epsilon) {
		hit.has_hit = false;
		return hit; //Ray parallel to triangle
	}
				
	float inv_det = 1.0 / det;
	
	s.set_vector(s.vector_substraction(r.origin, triangle.vertices[0]));
	
	float u = s.vector_dot(s, c) * inv_det;
				
	if ((u < 0 && fabsf(u) > epsilon) || (u > 1 && fabs(u - 1) > epsilon)) {
		hit.has_hit = false;
		return hit;
	}
				
	q.set_vector(q.vector_cross(s, edge1));
				
	float b = q.vector_dot(q, r.direction) * inv_det;
				
	if ((b < 0 && fabsf(b) > epsilon) || (u + b > 1 && fabsf(u + b + 1 ) > epsilon)) {
		hit.has_hit = false;
		return hit;
	}
				
	float t = q.vector_dot(edge2, q) * inv_det;
				
	if (t > epsilon) {
		hit.has_hit = true;
		hit.position = q.vector_addition(r.origin, q.vector_times_float(r.direction, t));
		return hit;
	}
	else {
		hit.has_hit = false;
		return hit;
	}
}



//Creates a PPM output file
void createPPM(Camera cam, Model model){
	vector<Facet> data = model.loadModel("model_cube.stl");
	
	int maxColors = 255;
	
	Color backgroundColor = {0, 0, 0}; //default schwarzer Hintergrund
	Color modelColor = {255, 255, 255}; //default pinkes Model
	
	ofstream ppm_file("output.ppm");
	
	//Add ppm header
	ppm_file << "P3" << endl;
	ppm_file << cam.get_imageWidth() << ' ' << cam.get_imageHeight() << endl;
	ppm_file << maxColors << endl;
	
	//Add model data to ppm
	for (int i = 0; i < cam.get_imageHeight(); i++){
		for (int j = 0; j < cam.get_imageWidth(); j++){
			
			for (int l = 0; l < int(data.size()); l++){
				Ray r = cam.get_ray(j, i);
				Facet tri = data.at(l);
				
				Hitpoint hit = intersection(tri, r);
				
				if (hit.has_hit == true){
					ppm_file << modelColor.r << ' ' << modelColor.g << ' ' << modelColor.b << endl;
					cout << "Hit at: " << hit.position.x << ' ' << hit.position.y << ' ' << hit.position.z << endl;
					break;
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
	
	cout << "Enter Camera Position X: ";
	cin >> camera.cameraPos.x;
	cout << "Enter Camera Position Y: ";
	cin >> camera.cameraPos.y;
	cout << "Enter Camera Position Z: ";
	cin >> camera.cameraPos.z;
	cout << "Enter Camera View X: ";
	cin >> camera.cameraView.x;
	cout << "Enter Camera View Y: ";
	cin >> camera.cameraView.y;
	cout << "Enter Camera View Z: ";
	cin >> camera.cameraView.z;
	
	createPPM(camera, testmodel);
	
	system("pause");
	
	return 0;
}

