#ifndef CAMERA_H
#define CAMERA_H
#include <vector>

using namespace std;

struct Pos3D {
	float x, y, z;
};

struct Pos2D {
	float x, y;
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
};



#endif
