#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <math.h>
#include <cmath>
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
		Vector3D barycentric;
		barycentric.set_values(t, u, v);
		hit.position.set_vector(barycentric_to_cartesian(barycentric, triangle.vertices[0], triangle.vertices[1], triangle.vertices[2]));
		
		hit.has_hit = true;
		hit.hit_color = triangle.vertexColor;
		hit.distance = t;
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
					triangleHit.facet_index = l;
					hitCounter++;
				
					if (triangleHit.distance < shortest_distance && triangleHit.distance > 0.0f){
						shortest_distance = triangleHit.distance;
						hit = triangleHit;
					}
				}
			}
			
			//Calculate Lighting
			Color lighted_color = hit.hit_color;
			if (hit.has_hit == true){
				Vector3D face_normal = data.at(hit.facet_index).normal;
				Vector3D hit_to_light = vector_subtraction(light.light_pos, hit.position);
				Vector3D hit_to_cam = vector_subtraction(r.origin, hit.position);
				Vector3D reflection = vector_subtraction(vector_multiplication(vector_times_float(vector_dot(hit_to_light, face_normal), 2), face_normal), hit_to_light);
			
				//Ambient
				lighted_color.r += int(light.ambient_const);
				lighted_color.g += int(light.ambient_const);
				lighted_color.b += int(light.ambient_const);
			
				//Diffuse
				lighted_color.r += int(light.diffuse_const * vector_dot(face_normal, hit_to_light));
				lighted_color.g += int(light.diffuse_const * vector_dot(face_normal, hit_to_light));
				lighted_color.b += int(light.diffuse_const * vector_dot(face_normal, hit_to_light));
			
				//Specular
				lighted_color.r += int(light.specular_const * pow(vector_dot(reflection, hit_to_cam), light.shininess));
				lighted_color.g += int(light.specular_const * pow(vector_dot(reflection, hit_to_cam), light.shininess));
				lighted_color.b += int(light.specular_const * pow(vector_dot(reflection, hit_to_cam), light.shininess));
			}
			else{
				lighted_color.r += light.ambient_const;
				lighted_color.g += light.ambient_const;
				lighted_color.b += light.ambient_const;
			}
			
			ppm_file << lighted_color.r << ' ' << lighted_color.g << ' ' << lighted_color.b << endl;
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
	cout << "Enter Light Position X: ";
	cin >> light.light_pos.x;
	cout << "Enter Light Position Y: ";
	cin >> light.light_pos.y;
	cout << "Enter Light Position Z: ";
	cin >> light.light_pos.z;
	
	createPPM(camera, model, light);
	
	system("pause");
	
	return 0;
}
