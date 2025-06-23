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

//Sum of two vectors
Vector3D vector_addition(Vector3D v1, Vector3D v2) {
	Vector3D v3;
	
	v3.x = v1.x + v2.x;
	v3.y = v1.y + v2.y;
	v3.z = v1.z + v2.z;
	
	return v3;
}

//Returns Subtraction of two Vertex
Vector3D vector_substraction(Vector3D v1, Vector3D v2) {
	Vector3D v3;
	
	v3.x = v1.x - v2.x;
	v3.y = v1.y - v2.y;
	v3.z = v1.z - v2.z;
	
	return v3;
}

//Multiply vector with a float
Vector3D vector_times_float(Vector3D v1, float f) {
	Vector3D v2;
	
	v2.x = v1.x * f;
	v2.y = v1.y * f;
	v2.z = v1.z * f;
	
	return v2;
}

//Returns dot product of two vectors
float vector_dot(Vector3D v1, Vector3D v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

//Returns cross product of two vectors
Vector3D vector_cross(Vector3D v1, Vector3D v2) {
	Vector3D v3;
	
	v3.x = v1.y * v2.z - v1.z * v2.y;
	v3.y = v1.z * v2.x - v1.x * v2.z;
	v3.z = v1.x * v2.y - v1.y * v2.x;
	
	return v3;
}

Hitpoint intersection(Facet triangle, Ray r, Camera cam){
	//Möller-Trumbore Intersection Algorithm
	Hitpoint hit;
	Vector3D edge1;
	Vector3D edge2;
	Vector3D c;
	Vector3D s;
	Vector3D q;
	
	edge1.set_vector(vector_substraction(triangle.vertices[1], triangle.vertices[0]));
	edge2.set_vector(vector_substraction(triangle.vertices[2], triangle.vertices[0]));
				
	c.set_vector(vector_cross(edge2, cam.cameraView));
	
	float det = vector_dot(edge1, c);
			
	constexpr float epsilon = numeric_limits<float>::epsilon();
				
	if (det > -epsilon && det < epsilon) {
		hit.has_hit = false;
		hit.hit_reason = 0;
		return hit; //Ray parallel to triangle or triangle is back-facing
	}
				
	float inv_det = 1.0 / det;
	
	s.set_vector(vector_substraction(cam.cameraPos, triangle.vertices[0]));
	
	float u = vector_dot(s, c) * inv_det;
				
	if (u < 0.0 || u > 1.0) {
		hit.has_hit = false;
		hit.hit_reason = 1;
		return hit;
	}
				
	q.set_vector(vector_cross(s, edge1));
				
	float b = vector_dot(q, cam.cameraPos) * inv_det;
				
	if (b < 0.0 || u + b > 1.0) {
		hit.has_hit = false;
		hit.hit_reason = 2;
		return hit;
	}
				
	float t = vector_dot(edge2, q) * inv_det;
				
	if (t < epsilon) {
		hit.has_hit = true;
		hit.position = vector_addition(cam.cameraPos, vector_times_float(r.direction, t));
		hit.hit_reason = 3;
		return hit;
	}
	else {
		hit.has_hit = false;
		hit.hit_reason = 4;
		return hit;
	}
}



//Creates a PPM output file
void createPPM(Camera cam, Model model){
	vector<Facet> data = model.loadModel("model_cube.stl");
	
	//TEST TRIANGLE----------------------
	Facet test_triangle;
	
	Vector3D p1;
	p1.set_values(0, 0, 0);
	Vector3D p2;
	p2.set_values(3, 0, 0);
	Vector3D p3;
	p3.set_values(0, 0, 3);
	
	Vector3D n;
	n.set_values(0, 1, 0);
	test_triangle.normal = n;
	
	
	test_triangle.vertices.push_back(p1);
	test_triangle.vertices.push_back(p2);
	test_triangle.vertices.push_back(p3);
	
	//------------------------------------
	
	int maxColors = 255;
	
	Color backgroundColor = {255, 0, 0}; //default roter Hintergrund
	Color modelColor = {255, 255, 255}; //default pinkes Model
	
	ofstream ppm_file("output.ppm");
	
	//Add ppm header
	ppm_file << "P3" << endl;
	ppm_file << cam.get_imageWidth() << ' ' << cam.get_imageHeight() << endl;
	ppm_file << maxColors << endl;
	
	//Counts the breaking points of intersect()
	int hitCounter = 0;
	int hitReason0 = 0;
	int hitReason1 = 0;
	int hitReason2 = 0;
	int hitReason3 = 0;
	int hitReason4 = 0;
	
	//Add model data to ppm
	for (int i = 0; i < cam.get_imageHeight(); i++){
		for (int j = 0; j < cam.get_imageWidth(); j++){
			//for (int l = 0; l < int(data.size()); l++){
				Ray r = cam.get_ray(j, i);
				//Facet tri = data.at(l);
				Facet tri = test_triangle;
				
				//Hitpoint hit = intersection(tri, r);
				Hitpoint hit = intersection(test_triangle, r, cam);
				
				if (hit.has_hit == true){					
					ppm_file << modelColor.r << ' ' << modelColor.g << ' ' << modelColor.b << endl;
					//cout << "Hit at: " << hit.position.x << ' ' << hit.position.y << ' ' << hit.position.z << endl;
					hitCounter++;
					switch (hit.hit_reason){
						case 0:
							hitReason0++;
							break;
						case 1:
							hitReason1++;
							break;
						case 2:
							hitReason2++;
							break;
						case 3:
							hitReason3++;
							break;
						case 4:
							hitReason4++;
							break;
						default:
							break;
					}
					
					continue;
				}
				else {
					ppm_file << backgroundColor.r << ' ' << backgroundColor.g << ' ' << backgroundColor.b << endl;
					
					switch (hit.hit_reason){
						case 0:
							hitReason0++;
							break;
						case 1:
							hitReason1++;
							break;
						case 2:
							hitReason2++;
							break;
						case 3:
							hitReason3++;
							break;
						case 4:
							hitReason4++;
							break;
						default:
							break;
					}
				}
			//}
		}
	}
	
	ppm_file.close();
	
	cout << "Output File created with " << hitCounter << " hits!" << endl;
	cout << "Parallel Rays: " << hitReason0 << endl;
	cout << "u < 0.0 || u > 1.0 -> " << hitReason1 << endl;
	cout << "b < 0.0 || u + b > 1.0 -> " << hitReason2 << endl;
	cout << "t > epsilon -> Hits -> " << hitReason3 << endl;
	cout << "t <= epsilon -> " << hitReason4 << endl;
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

