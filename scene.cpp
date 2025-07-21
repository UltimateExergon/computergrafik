#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <math.h>
#include <cmath>
#include <algorithm>

#include "model.h"
#include "camera.h"
#include "vector3d.h"
#include "light.h"
#include "cube.h"
#include "bmp.h"

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
		barycentric.set_values(1 - u - v, u, v);
		hit.baryPos.set_vector(barycentric);
		hit.position.set_vector(barycentric_to_cartesian(barycentric, triangle.vertices[0], triangle.vertices[1], triangle.vertices[2]));
		
		hit.has_hit = true;
		hit.distance = t;
		return hit;
	}
	else {	
		return hit;
	}
}

Color mapTexture(BMP texture, Hitpoint hit, Facet triangle){
	float u = hit.baryPos.x * triangle.uv[0].x + hit.baryPos.y * triangle.uv[1].x + hit.baryPos.z * triangle.uv[2].x;
	float v = hit.baryPos.x * triangle.uv[0].y + hit.baryPos.y * triangle.uv[1].y + hit.baryPos.z * triangle.uv[2].y;
	
	u *= texture.Dheader.width;
	v *= texture.Dheader.height;
	
	u = clamp(u, 0.0f, float(texture.Dheader.width));
	v = clamp(v, 0.0f, float(texture.Dheader.height));

	return texture.pixels.at(int(v) * texture.Dheader.width + int(u));
}

//Helper function to ensure proper color values
uint8_t clampColor(float c){
	uint8_t new_c = clamp(int(c), 0, 255);
	return new_c;
}

//Creates a PPM output file
void createPPM(Camera cam, Light light){
	Model model;
	auto triangles = model.loadModel("model.stl", "uv.txt");
	BMP texture = loadBMP("texture.bmp");
	
	UniformGrid grid(triangles, 20, 20, 20);
	grid.build(triangles);

	
	int maxColors = 255;
	
	ofstream ppm_file("output.ppm");
	
	//Add ppm header
	ppm_file << "P3" << endl;
	ppm_file << cam.get_imageWidth() << ' ' << cam.get_imageHeight() << endl;
	ppm_file << maxColors << endl;
	
	//Add model data to ppm
	for (unsigned int i = 0; i < cam.get_imageHeight(); i++){
		for (unsigned int j = 0; j < cam.get_imageWidth(); j++){
			Ray r = cam.get_ray(j, i);

            vector<int> candidates = grid.traverse(r);

            Hitpoint hit;
            float bestT = numeric_limits<float>::max();
            
            for (int tid : candidates) {
                Hitpoint triangleHit = intersection(triangles[tid], r);
                if (triangleHit.has_hit && triangleHit.distance < bestT) {
                    bestT = triangleHit.distance;
                    hit = triangleHit;
                    hit.facet_index = tid;
                }
            }
			
			//Calculate Lighting
			Color lighted_color = {1, 1, 1};
			
			if (hit.has_hit == true) {
				hit.hit_color = mapTexture(texture, hit, triangles.at(hit.facet_index));
				
				Vector3D face_normal = triangles.at(hit.facet_index).normal;
				Vector3D hit_to_light = normalize(vector_subtraction(light.light_pos, hit.position));
				Vector3D hit_to_cam = normalize(vector_subtraction(r.origin, hit.position));
				Vector3D reflection = normalize(vector_subtraction(vector_multiplication(vector_times_float(vector_dot(hit_to_light, face_normal), 2), face_normal), hit_to_light));
				
				//Ambient
				lighted_color.r = clampColor(lighted_color.r + light.light_color.r * light.ambient_const);
				lighted_color.g = clampColor(lighted_color.g + light.light_color.g * light.ambient_const);
				lighted_color.b = clampColor(lighted_color.b + light.light_color.b * light.ambient_const);
				
				//Diffuse
				float diffuseFactor = vector_dot(face_normal, hit_to_light);
				if (diffuseFactor > 0.0f){
					float diffuse = light.diffuse_const * diffuseFactor;
					
					lighted_color.r = clampColor(lighted_color.r + light.light_color.r * diffuse);
					lighted_color.g = clampColor(lighted_color.g + light.light_color.g * diffuse);
					lighted_color.b = clampColor(lighted_color.b + light.light_color.b * diffuse);
				}
				
				//Specular
				float specularFactor = clamp(vector_dot(reflection, hit_to_cam), 0.0f, 1.0f);
				if (specularFactor > 0.0f){
					float specular = light.specular_const * pow(specularFactor, light.shininess);
					
					lighted_color.r = clampColor(lighted_color.r + light.light_color.r * specular);
					lighted_color.g = clampColor(lighted_color.g + light.light_color.g * specular);
					lighted_color.b = clampColor(lighted_color.b + light.light_color.b * specular);
				}
			}
			
			//Final Color
			hit.hit_color.r = clampColor(hit.hit_color.r + lighted_color.r);
			hit.hit_color.g = clampColor(hit.hit_color.g + lighted_color.g);
			hit.hit_color.b = clampColor(hit.hit_color.b + lighted_color.b);
			
			ppm_file << hit.hit_color.r << ' ' <<  hit.hit_color.g << ' ' <<  hit.hit_color.b << endl;
		}
	}
	
	ppm_file.close();
	
	cout << "Output File created!" << endl;
}


int main() {
	cout << "Starting Programm" << endl;
	cout << "Program made by Judith <insert last name> and Julian Wolf" << endl;
	
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
	
	createPPM(camera, light);
	
	system("pause");
	
	return 0;
}
