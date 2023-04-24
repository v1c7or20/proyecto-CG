//
// Created by LAPTOP1 on 13/4/2023.
//

#ifndef CG2023_PLANO_H
#define CG2023_PLANO_H
#include "Objeto.h"

class Plano : public Objeto {
public:
    vec3 normal_plano;
    float d;
    Plano(vec3 normal, float dist, vec3 col): normal_plano(normal), Objeto(col, 1) {
        normal_plano.normalize();
    }
    bool intersectar(Rayo ray, float &t, vec3 &normal) {
        float denominador = normal_plano.punto(ray.dir);
        if (denominador != 0) {
            t = (normal_plano*d - ray.ori).punto(normal_plano) / denominador;
            if (t < 0) {
                return false;
            }
            normal = normal_plano;
            return true;
        }
        return false;
    }
};

#endif //CG2023_PLANO_H
