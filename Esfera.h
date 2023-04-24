//
// Created by LAPTOP1 on 8/4/2023.
//

#ifndef CG2023_ESFERA_H
#define CG2023_ESFERA_H
#include "Objeto.h"

class Esfera : public Objeto {
public:
    vec3 centro;
    float radio;

    Esfera(vec3 cen, float r, vec3 col): centro{cen},
    radio{r}, Objeto(col,1) {}

    Esfera(vec3 cen, float r, vec3 col, float kd):
    centro{cen}, radio{r}, Objeto(col,1)
    {this->k_difusa=kd;}

    bool intersectar(Rayo ray, float &t, vec3 &normal) override {
        auto _a = ray.dir.punto(ray.dir);
        auto _b = 2*ray.dir.punto(ray.ori-centro);
        auto _c = (ray.ori-centro).punto(ray.ori-centro)-radio*radio;
        auto D = _b*_b-4*_a*_c;
        if(D <= 0) {return false;}
        float t1 = (-_b + sqrt(D))/2*_a;
        float t2 = (-_b - sqrt(D))/2*_a;
        t = std::min(t1, t2);
        if(t <= 0) {return false;}
        vec3 pi = ray.ori + ray.dir * t;
        normal = pi - centro;
        normal.normalize();
        return true;

    }
};


#endif //CG2023_ESFERA_H
