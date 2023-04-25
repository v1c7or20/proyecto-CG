//
// Created by hgallegos on 30/03/2023.
//

#ifndef CG2023_OBJETO_H
#define CG2023_OBJETO_H
#include "vec3.h"
#include "Rayo.h"

class Objeto {
public:
    vec3 color;
    float k_difusa=0.8f; // contstante difusa
    float k_especular=0.8f; // constante especula
    int n = 32;
    float k_espejo; // constante espejo
    bool es_transparente;
    float index_refraction; // index of refraction


    Objeto(vec3 color): color{color}{ k_espejo = 0.7f;}
    Objeto(vec3 color, float kdifusa=1): color{color},
                                         k_difusa{kdifusa}{ k_espejo = 0; es_transparente=false; index_refraction=1; }

    void setConstantes(float kdifusa=1, float kespecular=1, float n=8)
    {this->k_difusa=kdifusa; this->k_especular=kespecular; this->n = n;}

    virtual bool intersectar(Rayo ray, float &t, vec3 &normal)=0;
    virtual void moveUp(float)=0;
    virtual void moveDown(float)=0;
    virtual void moveLeft()=0;
    virtual void moveRight()=0;
};

#endif //CG2023_OBJETO_H
