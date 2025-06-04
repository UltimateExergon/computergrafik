#include "ray.h"

Ray::Ray() : origin{0, 0, 0}, direction{0, 0, 0} {}

Ray::Ray(Pos3D orig, Pos3D dir) : origin(orig), direction(dir) {}

