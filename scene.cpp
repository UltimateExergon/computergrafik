#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <math.h>
#include "model.h"
#include "camera.h"
#include "vector3d.h"
#include "light.h"

using namespace std;

Hitpoint intersection(Facet triangle, Ray r){
    Hitpoint hit;
    Vector3D edge1;
    Vector3D edge2;
    Vector3D c;
    Vector3D s;
    Vector3D q;

    edge1.set_vector(vector_subtraction(triangle.vertices[1], triangle.vertices[0]));
    edge2.set_vector(vector_subtraction(triangle.vertices[2], triangle.vertices[0]));

    c.set_vector(vector_cross(r.direction, edge2));

    float det = vector_dot(edge1, c);
    
	constexpr float epsilon = numeric_limits<float>::epsilon();
	
	 if (det > -epsilon && det < epsilon) {
        return hit; //Ray parallel to triangle or triangle is back-facing
    }

    float inv_det = 1.0f / det;

    s.set_vector(vector_subtraction(r.origin, triangle.vertices[0]));

    float u = vector_dot(s, c) * inv_det;

    if (u < 0.0f || u > 1.0f) {
        return hit;
    }

    q.set_vector(vector_cross(s, edge1));

    float v = vector_dot(q, r.direction) * inv_det;

    if (v < 0.0f || u + v > 1.0f) {
        return hit;
    }

    float t = vector_dot(edge2, q) * inv_det;

    if (t > epsilon) {
		hit.position = vector_addition(r.origin, vector_times_float(r.direction, t));
		hit.has_hit = true;
		hit.hit_color = triangle.vertexColor;
		hit.hit_point = t;
		return hit;
	}
	else {	
		return hit;
	}
}



//Creates a PPM output file
void createPPM(Camera cam, Model model, Light light){
	vector<Facet> data = model.loadModel("model_cube.stl", "colors.txt");
	
	int maxColors = 255;
	
	ofstream ppm_file("output.ppm");
	
	//Add ppm header
	ppm_file << "P3" << endl;
	ppm_file << cam.get_imageWidth() << ' ' << cam.get_imageHeight() << endl;
	ppm_file << maxColors << endl;
	
	//Counts the hits
	int hitCounter = 0;
	
	//Add model data to ppm
	for (int i = 0; i < cam.get_imageHeight(); i++){
		for (int j = 0; j < cam.get_imageWidth(); j++){
			
			Hitpoint hit;
			Ray r = cam.get_ray(j, i);
			
			float shortest_distance = numeric_limits<float>::max();
			
			for (int l = 0; l < int(data.size()); l++){
				Hitpoint triangleHit;
			
				triangleHit = intersection(data.at(l), r);
				if (triangleHit.has_hit == true){
					float dist = triangleHit.hit_point;
					hitCounter++;
				
					if (dist < shortest_distance && dist > 0.0f){
						shortest_distance = dist;
						hit = triangleHit;
					}
				}
			}
			
			ppm_file << hit.hit_color.r << ' ' << hit.hit_color.g << ' ' << hit.hit_color.b << endl;
		}
	}
	
	ppm_file.close();
	
	cout << "Output File created with " << hitCounter << " hits!" << endl;
}


int main() {
	cout << "Starting Programm" << endl;
	
	Model model;
	Camera camera;
	Light light;
	
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
	
	createPPM(camera, model, light);
	
	system("pause");
	
	return 0;
}
