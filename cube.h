#ifndef CUBE_H
#define CUBE_H
#include <vector>
#include "vector3d.h"
#include "model.h"
#include "camera.h"

using namespace std;

using Cell = vector<int>;

/*
uniform grid = Aufteilung in Würfelzellen
AABB = axis-aligned bounding box
*/
struct UniformGrid {
    Vector3D bbMin, bbMax;
    int nx, ny, nz;
    Vector3D cellSize;
    vector<Cell> cells;

    UniformGrid(const vector<Facet>& tris, int resX, int resY, int resZ);
    void build(const vector<Facet>& tris);

    vector<int> traverse(const Ray& r) const;

    private: 
    
    inline int flatten(int i, int j, int k) const {
        return i + nx*(j + ny*k);
    }
    Vector3D worldToGrid(const Vector3D& p) const;

};

#endif
