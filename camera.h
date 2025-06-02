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

class Camera{
	public:
		Pos3D cameraPos;
		Pos3D cameraView;
		Pos2D worldSize;
		int imageWidth = 1000;
		int imageHeight = 1000;
		
		Camera();
		int get_imageHeight();
		int get_imageWidth();
};

#endif
