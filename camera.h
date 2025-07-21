#ifndef CAMERA_H
#define CAMERA_H
#include <vector>
#include <limits>
#include "vector3d.h"

using namespace std;

class Ray {
	public:
		Vector3D origin;
		Vector3D direction;

		Ray();
		Ray(Vector3D origin, Vector3D direction);
};

class Hitpoint{
	public:
		Vector3D position; //Cartesian Coordinates of the Point
		Vector3D baryPos; //Barycentric coordinates of the point
		float distance; 
		bool has_hit;
		Color hit_color;
		unsigned int facet_index;
		
		Hitpoint();
		float calculate_distance(Vector3D v2);
};

class Camera {
public:
    Vector3D cameraPos;  //Position of Camera -> Location the Camera is looking from 
    Vector3D cameraView; //The Point the Camera is looking at 
	
	// statt worldSize:
    float screenWidth;  
    float screenHeight;

    unsigned int imageWidth; // jetzt im Konstruktor initialisiert
    unsigned int imageHeight; // jetzt im Konstruktor initialisiert
    
	float offsetX;
	float offsetY;

    Camera();
    unsigned int get_imageHeight();
    unsigned int get_imageWidth();
	// wandelt Pixelkoordinaten in Weltkoordinaten um
    Vector3D get_pixel(int x, int y);
    Ray get_ray(int x, int y);
};

#endif
