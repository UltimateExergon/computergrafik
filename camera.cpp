#include <cmath>
#include <iostream>
#include "camera.h"

using namespace std;

Camera::Camera() {
	cameraPos.x = 10;
	cameraPos.y = 10;
	cameraPos.z = 10;
	
	cameraView.x = 0;
	cameraView.y = 0;
	cameraView.z = 0;
	
    screenWidth = 2.0f;
    screenHeight = 2.0f;
    imageWidth = 500; 
    imageHeight = 500;
}


int Camera::get_imageWidth(){
	return imageWidth;
}

int Camera::get_imageHeight(){
	return imageHeight;
}


Pos3D Camera::get_pixel(int x, int y) const {
    // Berechne den Mittelpunkt des Bildschirms in Weltkoordinaten
    Pos3D screenCenter = { cameraPos.x + cameraView.x, 
                           cameraPos.y + cameraView.y, 
                           cameraPos.z + cameraView.z };

    // Berechne die Größe eines Pixels in Weltkoordinaten
    float pixelWidth = screenWidth / imageWidth;
    float pixelHeight = screenHeight / imageHeight;

    // Berechne die Offset-Werte zur Position des Pixels relativ zur Mitte
    float offsetX = (x - imageWidth / 2.0f) * pixelWidth;
    float offsetY = (y - imageHeight / 2.0f) * pixelHeight;

    // Berechne die endgültige Pixelposition
    Pos3D pixelPos = { screenCenter.x + offsetX, 
                       screenCenter.y + offsetY, 
                       screenCenter.z };

    return pixelPos;
}

Ray Camera::get_ray(int x, int y) const {
    Pos3D pixelPos = get_pixel(x, y);

    Pos3D direction = { pixelPos.x - cameraPos.x, 
                        pixelPos.y - cameraPos.y, 
                        pixelPos.z - cameraPos.z };

    float length = sqrt(direction.x * direction.x + 
                        direction.y * direction.y + 
                        direction.z * direction.z);

    direction.x /= length;
    direction.y /= length;
    direction.z /= length;

    return Ray(cameraPos, direction);
}

Ray::Ray() : origin{0, 0, 0}, direction{0, 0, 0} {}
Ray::Ray(Pos3D orig, Pos3D dir) : origin(orig), direction(dir) {}

Hitpoint::Hitpoint(){};

