#include <cmath>
#include <iostream>
#include "camera.h"
#include "vector3d.h"

using namespace std;

Camera::Camera() {
    screenWidth = 10.0f;
    screenHeight = 10.0f;
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
    // Berechne den Mittelpunkt des Bildschirms in Weltkoordinaten
    Vector3D screenCenter;
    screenCenter.x = cameraPos.x + cameraView.x;
    screenCenter.y = cameraPos.y + cameraView.y; 
    screenCenter.z = cameraPos.z + cameraView.z;
	//screenCenter = cameraPos;

    // Berechne die Größe eines Pixels in Weltkoordinaten
    float pixelWidth = screenWidth / imageWidth;
    float pixelHeight = screenHeight / imageHeight;
    
	Vector3D upperLeftCorner;
	upperLeftCorner.x = cameraPos.x - pixelWidth/2 - pixelHeight/2; 
	upperLeftCorner.y = cameraPos.y - pixelWidth/2 - pixelHeight/2; 
	upperLeftCorner.z = cameraPos.z - pixelWidth/2 - pixelHeight/2; 
	
	 // Berechne die Offset-Werte zur Position des Pixels relativ zur Mitte
    offsetX = (x - imageWidth / 2.0f) * pixelWidth;
    offsetY = (y - imageHeight / 2.0f) * pixelHeight;
    
    //offsetX = pixelWidth;
    //offsetY = pixelHeight;
	
	//pixel00loc.x = upperLeftCorner.x + 0.5 * (offsetX + offsetY);
	//pixel00loc.y = upperLeftCorner.y + 0.5 * (offsetX + offsetY);
	//pixel00loc.z = upperLeftCorner.z + 0.5 * (offsetX + offsetY);

    // Berechne die endgültige Pixelposition
    Vector3D pixelPos;
    pixelPos.x = screenCenter.x + offsetX;
    pixelPos.y = screenCenter.y + offsetY; 
    pixelPos.z = screenCenter.z;

    return pixelPos;
}

Ray Camera::get_ray(int x, int y) {
    Vector3D pixelPos = get_pixel(x, y);

    Vector3D direction;
    //direction.x = pixel00loc.x + x + offsetX;
    //direction.y = pixel00loc.y + y + offsetY;
    //direction.z = pixel00loc.z;
    
    direction.x = pixelPos.x - cameraPos.x; 
    direction.y = pixelPos.y - cameraPos.y; 
    direction.z = pixelPos.z - cameraPos.z;

    float length = sqrt(direction.x * direction.x + 
                        direction.y * direction.y + 
                        direction.z * direction.z);

    direction.x /= length;
    direction.y /= length;
    direction.z /= length;

    return Ray(cameraPos, direction);
}

Vector3D Ray::normalize(Vector3D v1){
	Vector3D v2;
	
	float length = sqrt(v1.x * v1.x + v1.y * v1.y + v2.z * v2.z);
	v2.x = v2.x / length;
	v2.y = v2.y / length;
	v2.z = v2.z / length;
	
	return v2;
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

