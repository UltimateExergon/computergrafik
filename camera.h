#ifndef CAMERA_H
#define CAMERA_H
#include <vector>
#include <limits>
#include "model.h"

using namespace std;

struct Pos3D {
	float x, y, z;
};

struct Pos2D {
	float x, y;
};

class Ray {
	public:
		Pos3D origin;
		Pos3D direction;

		Ray();
		Ray(Pos3D origin, Pos3D direction);
		Pos3D normalize(Pos3D v1);
};

class Hitpoint{
	public:
		Pos3D position;
		float distance = numeric_limits<float>::max();
		const Model* model = nullptr;
		
		Hitpoint();
};

class Camera {
public:
    Pos3D cameraPos;   
    Pos3D cameraView;  
	
	// statt worldSize:
    float screenWidth;  
    float screenHeight;

    int imageWidth; // jetzt im Konstruktor initialisiert
    int imageHeight; // jetzt im Konstruktor initialisiert

    Camera();
    int get_imageHeight();
    int get_imageWidth();
	// wandelt Pixelkoordinaten in Weltkoordinaten um
    Pos3D get_pixel(int x, int y) const;
    Ray get_ray(int x, int y) const;
};

#endif
