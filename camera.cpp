#include <cmath>
#include <iostream>
#include <cmath>
#include "camera.h"
#include "vector3d.h"

using namespace std;

Camera::Camera() {
    screenWidth = 4.0f;
    screenHeight = 4.0f;
    imageWidth = 1400; 
    imageHeight = 1300;
    
	offsetX = 0.0f;
	offsetY = 0.0f;
}


unsigned int Camera::get_imageWidth(){
	return imageWidth;
}

unsigned int Camera::get_imageHeight(){
	return imageHeight;
}


Vector3D Camera::get_pixel(int x, int y) {
    Vector3D forward = normalize(cameraView);
    Vector3D tmpUp(0, 0, 1);

    if (fabs(vector_dot(forward, tmpUp)) > 0.999f) {
        tmpUp = Vector3D(1, 0, 0);
    }

    Vector3D right = normalize(vector_cross(tmpUp, forward));
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
    Vector3D dir = normalize(vector_subtraction(pixelPos, cameraPos));
    
    return Ray(cameraPos, dir);
}

Ray::Ray(){}


Ray::Ray(Vector3D orig, Vector3D dir) : origin(orig), direction(dir) {}

Hitpoint::Hitpoint(){
	distance = numeric_limits<float>::max();
	hit_color = {177, 127, 77};
	has_hit = false;
};

float Hitpoint::calculate_distance(Vector3D v2){
	return sqrt(pow((position.x - v2.x), 2) + pow((position.y - v2.y), 2) + pow((position.z - v2.z), 2));
}

