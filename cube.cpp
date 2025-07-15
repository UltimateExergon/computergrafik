#include "cube.h"
#include <algorithm>
#include <limits>
#include <cmath>

using namespace std;

// baut AABB auf
UniformGrid::UniformGrid(const vector<Facet>& tris, int resX, int resY, int resZ) : nx(resX), ny(resY), nz(resZ)
{
    bbMin = {  numeric_limits<float>::infinity(),
               numeric_limits<float>::infinity(),
               numeric_limits<float>::infinity() };

    bbMax = { -numeric_limits<float>::infinity(),
              -numeric_limits<float>::infinity(),
              -numeric_limits<float>::infinity() };

    for (auto& f : tris)
        for (int v = 0; v < 3; ++v) {
            bbMin.x = min(bbMin.x, f.vertices[v].x);
            bbMin.y = min(bbMin.y, f.vertices[v].y);
            bbMin.z = min(bbMin.z, f.vertices[v].z);
            bbMax.x = max(bbMax.x, f.vertices[v].x);
            bbMax.y = max(bbMax.y, f.vertices[v].y);
            bbMax.z = max(bbMax.z, f.vertices[v].z);
        }

    cellSize = {
        (bbMax.x - bbMin.x) / nx,
        (bbMax.y - bbMin.y) / ny,
        (bbMax.z - bbMin.z) / nz
    };

    cells.clear();
    cells.resize(nx * ny * nz);
}

// Umwandlung in Gridkoordinaten also quasi welche Zelle und welche relative Position innerhalb dieser Zelle
Vector3D UniformGrid::worldToGrid(const Vector3D& p) const {
    return {
        (p.x - bbMin.x) / cellSize.x,
        (p.y - bbMin.y) / cellSize.y,
        (p.z - bbMin.z) / cellSize.z
    };
}

void UniformGrid::build(const std::vector<Facet>& tris) {
    // für jedes Dreieck...
    for (int t = 0; t < int(tris.size()); ++t) {
        
        // ... wird AABB bestimmt
        Vector3D tmin = tris[t].vertices[0], tmax = tmin;
        for (int v=1; v<3; ++v) {
            tmin.x = min(tmin.x, tris[t].vertices[v].x);
            tmin.y = min(tmin.y, tris[t].vertices[v].y);
            tmin.z = min(tmin.z, tris[t].vertices[v].z);
            tmax.x = max(tmax.x, tris[t].vertices[v].x);
            tmax.y = max(tmax.y, tris[t].vertices[v].y);
            tmax.z = max(tmax.z, tris[t].vertices[v].z);
        }

        // und das wird dann übersetzt in Indizes
        Vector3D gmin = worldToGrid(tmin), gmax = worldToGrid(tmax);
        int i0 = clamp(int(floor(gmin.x)), 0, nx-1);
        int j0 = clamp(int(floor(gmin.y)), 0, ny-1);
        int k0 = clamp(int(floor(gmin.z)), 0, nz-1);
        int i1 = clamp(int(floor(gmax.x)), 0, nx-1);
        int j1 = clamp(int(floor(gmax.y)), 0, ny-1);
        int k1 = clamp(int(floor(gmax.z)), 0, nz-1);

        // und dann durchläuft man das für alle Zellen, die das AABB des Dreiecks berühren
        for (int k=k0; k<=k1; ++k)
         for (int j=j0; j<=j1; ++j)
          for (int i=i0; i<=i1; ++i)
            cells[flatten(i,j,k)].push_back(t);
    }
}

vector<int> UniformGrid::traverse(const Ray& r) const {
    vector<int> result;

    // slab-Methode
    float t0 = 0, t1 = numeric_limits<float>::infinity();
    for (int ax = 0; ax < 3; ++ax) {
        float invD = 1.0f / (ax==0 ? r.direction.x
                                   : ax==1 ? r.direction.y
                                           : r.direction.z);
        float tNear = ((ax==0 ? bbMin.x : ax==1 ? bbMin.y : bbMin.z)
                       - (ax==0 ? r.origin.x : ax==1 ? r.origin.y : r.origin.z))
                      * invD;
        float tFar  = ((ax==0 ? bbMax.x : ax==1 ? bbMax.y : bbMax.z)
                       - (ax==0 ? r.origin.x : ax==1 ? r.origin.y : r.origin.z))
                      * invD;
        if (tNear > tFar) std::swap(tNear, tFar);
        t0 = max(t0, tNear);
        t1 = min(t1, tFar);
        if (t0 > t1) return result;
    }

    Vector3D p = { r.origin.x + t0 * r.direction.x,
                   r.origin.y + t0 * r.direction.y,
                   r.origin.z + t0 * r.direction.z };
                   
    // wenn der Strahl trifft, wird er umgewandelt und Startzelle bestimmt
    Vector3D g = worldToGrid(p);
    int i = clamp(int(floor(g.x)), 0, nx-1);
    int j = clamp(int(floor(g.y)), 0, ny-1);
    int k = clamp(int(floor(g.z)), 0, nz-1);

    int stepX = (r.direction.x > 0) ? 1 : -1;
    int stepY = (r.direction.y > 0) ? 1 : -1;
    int stepZ = (r.direction.z > 0) ? 1 : -1;

    auto compute = [&](float originC, float dirC, float bbMinC, float cs,
                       int gi, int step){
        float tDelta = fabs(cs / dirC);
        float border = bbMinC + (gi + (step>0)) * cs;
        float tNext  = (border - originC) / dirC;
        return std::pair<float,float>(tDelta, tNext);
    };

    auto [dtx, tx] = compute(r.origin.x, r.direction.x, bbMin.x, cellSize.x, i, stepX);
    auto [dty, ty] = compute(r.origin.y, r.direction.y, bbMin.y, cellSize.y, j, stepY);
    auto [dtz, tz] = compute(r.origin.z, r.direction.z, bbMin.z, cellSize.z, k, stepZ);

    float t = t0;
    
    //alle Zellen durchwandern solange noch im Raster/AABB
    while (t <= t1) {
        for (int tid : cells[flatten(i,j,k)])
            result.push_back(tid);

        if (tx < ty && tx < tz) {
            i += stepX; t = tx; tx += dtx;
        }
        else if (ty < tz) {
            j += stepY; t = ty; ty += dty;
        }
        else {
            k += stepZ; t = tz; tz += dtz;
        }
        if (i<0||i>=nx||j<0||j>=ny||k<0||k>=nz) break;
    }

    // Duplikate entfernen (weil Dreiecke in beanchbarten Zellen doppelt auftreten können)
    sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());

    return result;
}
