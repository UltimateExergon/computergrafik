#ifndef CAMERA_H
#define CAMERA_H
#include <vector>
#include <limits>
#include "model.h"
#include "vector3d.h"

using namespace std;

struct Pos2D {
	float x, y;
};

class Ray {
	public:
		Vector3D origin;
		Vector3D direction;

		Ray();
		Ray(Vector3D origin, Vector3D direction);
		Vector3D normalize(Vector3D v1);
};

class Hitpoint{
	public:
		Vector3D position;
		float distance = numeric_limits<float>::max();
		bool has_hit;
		const Model* model = nullptr;
		int hit_reason;
		
		Hitpoint();
};

class Camera {
public:
    Vector3D cameraPos;  //Position of Camera -> Location the Camera is looking from 
    Vector3D cameraView; //The Point the Camera is looking at 
	
	// statt worldSize:
    float screenWidth;  
    float screenHeight;

    int imageWidth; // jetzt im Konstruktor initialisiert
    int imageHeight; // jetzt im Konstruktor initialisiert
    
    Vector3D pixel00loc;
	float offsetX;
	float offsetY;

    Camera();
    int get_imageHeight();
    int get_imageWidth();
	// wandelt Pixelkoordinaten in Weltkoordinaten um
    Vector3D get_pixel(int x, int y);
    Ray get_ray(int x, int y);
};

#endif
