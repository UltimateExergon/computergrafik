#include "camera.h"
#ifndef RAY_H
#define RAY_H


class Ray {
public:
    Pos3D origin;
    Pos3D direction;

    Ray();
    Ray(Pos3D origin, Pos3D direction);
};

#endif
