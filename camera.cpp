#include <cmath>
#include <iostream>
#include "camera.h"
#include "vector3d.h"

using namespace std;

Camera::Camera() {
    screenWidth = 4.0f; // vorher: 2.0f
    screenHeight = 4.0f; // vorher: 2.0f
    imageWidth = 200; 
    imageHeight = 200;
    
	offsetX = 0.0f;
	offsetY = 0.0f;
}


int Camera::get_imageWidth(){
	return imageWidth;
}

int Camera::get_imageHeight(){
	return imageHeight;
}


Vector3D Camera::get_pixel(int x, int y) {
    Vector3D forward = vector_normalize(cameraView);
    Vector3D tmpUp(0, 0, 1);

    if (fabs(vector_dot(forward, tmpUp)) > 0.999f) {
        tmpUp = Vector3D(1, 0, 0);
    }

    Vector3D right = vector_normalize(vector_cross(tmpUp, forward));
    Vector3D up = vector_cross(forward, right);

    float pixelWidth = screenWidth / imageWidth;
    float pixelHeight = screenHeight / imageHeight;

    float sx = (x + 0.5f - imageWidth / 2.0f) * pixelWidth;
    float sy = (y + 0.5f - imageHeight / 2.0f) * pixelHeight;

    // Jetzt korrekt: Kamera schaut entlang 'forward', und Bildfläche liegt senkrecht dazu
    Vector3D screenCenter = vector_addition(cameraPos, forward);

    // Die Pixelposition liegt seitlich (right) und oben (up) versetzt vom screenCenter
    Vector3D pixelPos = vector_addition(
        vector_addition(screenCenter, vector_times_float(right, sx)),
        vector_times_float(up, sy)
    );

    return pixelPos;
}



Ray Camera::get_ray(int x, int y) {
    Vector3D pixelPos = get_pixel(x, y);
    Vector3D dir = vector_subtraction(pixelPos, cameraPos);
    
    float length = sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
    dir.x /= length;
    dir.y /= length;
    dir.z /= length;

    return Ray(cameraPos, dir);
}


Vector3D Ray::normalize(Vector3D v1) {
    float length = sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);  // v1.z statt v2.z
    if (length == 0) return v1;
    v1.x /= length;
    v1.y /= length;
    v1.z /= length;
    return v1;
}


Ray::Ray(){
	origin.x = 0;
	origin.y = 0;
	origin.z = 0;
	
	direction.x = 0;
	direction.y = 0;
	direction.z = 0;
}


Ray::Ray(Vector3D orig, Vector3D dir) : origin(orig), direction(dir) {}

Hitpoint::Hitpoint(){};

