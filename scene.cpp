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
		barycentric.set_values(t, u, v);
		hit.baryPos.set_vector(barycentric);
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

Color mapTexture(BMP texture, Hitpoint hit, Facet triangle){
	//there is probably a better way to do this, but I don't have the time right now
	
	Color textureColor;
	
	//get triangles region on the texture
	auto minW;
	auto maxW;
	auto minH;
	auto maxH;
	
	//upper left
	if (hit.facet_index < 2) {
		minW = 0;
		maxW = texture.Dheader.width / 3;
		minH = 0;
		maxH = texture.Dheader.height / 2;
	}
	//upper middle
	else if (hit.facet_index < 4) {
		minW = (texture.Dheader.width / 3) + 1;
		maxW = texture.Dheader.width - (texture.Dheader.width / 3);
		minH = 0;
		maxH = texture.Dheader.height / 2;
	}
	//upper right
	else if (hit.facet_index < 6){
		maxW = texture.Dheader.width - (texture.Dheader.width / 3) + 1;
		maxW = texture.Dheader.width;
		minH = 0;
		maxH = texture.Dheader.height / 2;
	}
	//bottom left
	else if (hit.facet_index < 8) {
		minW = 0;
		maxW = texture.Dheader.width / 3;
		minH = maxH = (texture.Dheader.height / 2) + 1;
		maxH = texture.Dheader.height;
	}
	//bottom middle
	else if (hit.facet_index < 10){
		minW = (texture.Dheader.width / 3) + 1;
		maxW = texture.Dheader.width - (texture.Dheader.width / 3);
		minH = maxH = (texture.Dheader.height / 2) + 1;
		maxH = texture.Dheader.height;
	}
	//bottom right
	else {
		maxW = texture.Dheader.width - (texture.Dheader.width / 3) + 1;
		maxW = texture.Dheader.width;
		minH = maxH = (texture.Dheader.height / 2) + 1;
		maxH = texture.Dheader.height;
	}
	
	vector<int> texture_triangle;
	// use upper triangle
	if (hit.facet_index % 2){
		Vector2D p1;
		p1.x = minW;
		p1.y = maxH;
		
		Vector2D p2;
		p2.x = minW;
		p2.y = maxH;
		
		Vector2D p3;
		p3.x = maxW;
		p3.y = minH;
		
		texture_triangle.push_back(p1);
		texture_triangle.push_back(p2);
		texture_triangle.push_back(p3);
	}
	//use lower triangle
	else {
		Vector2D p1;
		p1.x = maxW;
		p1.y = maxH;
		
		Vector2D p2;
		p2.x = minW;
		p2.y = maxH;
		
		Vector2D p3;
		p3.x = maxW;
		p3.y = minH;
		
		texture_triangle.push_back(p1);
		texture_triangle.push_back(p2);
		texture_triangle.push_back(p3);
	}
	
	//Determine scale factors of the sides of the two triangles
	//(ignore z coordinate)
	Vector3D scale_factors;
	
	float texture_triangle_sideLength1 = length2D(p2-p1);
	float texture_triangle_sideLength2 = length2D(p3-p2);
	float texture_triangle_sideLength3 = length2D(p3-p1);
	
	Vector2D model_triangleP1 = {triangle.vertices.at(0).x, triangle.vertices.at(0).y};
	Vector2D model_triangleP2 = {triangle.vertices.at(1).x, triangle.vertices.at(1).y};
	Vector2D model_triangleP3 = {triangle.vertices.at(2).x, triangle.vertices.at(2).y};
	
	float model_triangle_sideLength1 = length2D(vector_subtraction2D(model_triangleP2, model_triangleP1));
	float model_triangle_sideLength2 = length2D(vector_subtraction2D(model_triangleP2, model_triangleP3));
	float model_triangle_sideLength3 = length2D(vector_subtraction2D(model_triangleP3, model_triangleP1));
	
	scale_factors.x = texture_triangle_sideLength1 / model_triangle_sideLength1;
	scale_factors.y = texture_triangle_sideLength2 / model_triangle_sideLength2;
	scale_factors.z = texture_triangle_sideLength3 / model_triangle_sideLength3;
	
	//Multiply hit point barycentric coordinates with the scale factors
	Vector3D newBarycentric = set_vector(vector_multiplication(hit.baryPos, scale_factors));
	
	//Convert new barycentric coordinates to cartesian
	Vector3D newCartesian = set_vector(barycentric_to_cartesian(newBarycentric, ));
	
	//get the texture's color at the cartesian coordinates
	textureColor = getPixel(texture, newCartesian.x, newCartesian.y);
	
	return textureColor;
}

//Creates a PPM output file
void createPPM(Camera cam, Model model, Light light, BMP texture){
	auto triangles = model.loadModel("model_cube.stl");
	texture = loadBMP("texture.bmp");
	
	UniformGrid grid(triangles, 20, 20, 20);
	grid.build(triangles);

	
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
			Ray r = cam.get_ray(j, i);

            std::vector<int> candidates = grid.traverse(r);

            Hitpoint hit;
            float bestT = numeric_limits<float>::max();
            for (int tid : candidates) {
                Hitpoint triangleHit = intersection(triangles[tid], r);
                if (triangleHit.has_hit && triangleHit.distance < bestT) {
                    bestT = triangleHit.distance;
                    hit = triangleHit;
                    hit.facet_index = tid;
                    hit.hit_color = mapTexture(texture, hit, triangles[tid]);
                }
            }

			
			//Calculate Lighting
			Color lighted_color = hit.hit_color;
			if (hit.has_hit == true){
				Vector3D face_normal = triangles.at(hit.facet_index).normal;
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
	cout << "Program made by Judith <insert last name> and Julian Wolf" << endl;
	
	Model model;
	Camera camera;
	Light light;
	BMP texture;
	
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
	
	createPPM(camera, model, light, texture);
	
	system("pause");
	
	return 0;
}
