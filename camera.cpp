#include <cmath>
#include "camera.h"

Camera::Camera() {
    screenWidth = 2.0f;
    screenHeight = 2.0f;
    imageWidth = 1000; 
    imageHeight = 1000;
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


