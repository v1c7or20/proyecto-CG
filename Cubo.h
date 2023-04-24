//
// Created by LAPTOP1 on 15/4/2023.
//

#ifndef CG2023_CUBO_H
#define CG2023_CUBO_H

#include "Objeto.h"

class Cubo : public Objeto {
public:
    vec3 minimo;
    vec3 maximo;

    Cubo(vec3 min, vec3 max, vec3 col) : minimo{ min }, maximo{ max }, Objeto(col, 1) {}
    bool intersectar(Rayo ray, float &t, vec3 &normal) {
        float tx1 = (minimo.x - ray.ori.x) / ray.dir.x;
        float tx2 = (maximo.x - ray.ori.x) / ray.dir.x;
        float tmin = std::min(tx1, tx2);
        float tmax = std::max(tx1, tx2);

        float ty1 = (minimo.y - ray.ori.y) / ray.dir.y;
        float ty2 = (maximo.y - ray.ori.y) / ray.dir.y;
        tmin = std::max(tmin, std::min(ty1, ty2));
        tmax = std::min(tmax, std::max(ty1, ty2));

        float tz1 = (minimo.z - ray.ori.z) / ray.dir.z;
        float tz2 = (maximo.z - ray.ori.z) / ray.dir.z;
        tmin = std::max(tmin, std::min(tz1, tz2));
        tmax = std::min(tmax, std::max(tz1, tz2));

        if (tmax < 0 || tmin > tmax) {
            return false;
        }

        t = tmin;
        vec3 pi = ray.ori + ray.dir * t;

        if (pi.x == minimo.x) {
            normal = vec3(-1, 0, 0);
        } else if (pi.x == maximo.x) {
            normal = vec3(1, 0, 0);
        } else if (pi.y == minimo.y) {
            normal = vec3(0, -1, 0);
        } else if (pi.y == maximo.y) {
            normal = vec3(0, 1, 0);
        } else if (pi.z == minimo.z) {
            normal = vec3(0, 0, -1);
        } else if (pi.z == maximo.z) {
            normal = vec3(0, 0, 1);
        }

        return true;
    }
};
#endif //CG2023_CUBO_H
