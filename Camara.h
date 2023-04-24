//
// Created by hgallegos on 30/03/2023.
//

#ifndef CG2023_CAMARA_H
#define CG2023_CAMARA_H

#include "Rayo.h"
#include "vec3.h"
#include "CImg.h"
#include "Esfera.h"
#include "Plano.h"
#include "Cubo.h"
#include "Cilindro.h"
#include "Luz.h"
#include <string>

using namespace cimg_library;
typedef unsigned char BYTE;

class Camara {
    vec3 eye, xe, ye, ze;
    float f, a, b, w, h;
    CImg<BYTE> *pImg = nullptr;
    int prof_max=4;
public:
    void configurar(float _near, float fov, int ancho, int alto,
                    vec3 pos_eye, vec3 center, vec3 up);
    void renderizar(std::vector<Objeto *> &objetos, std::vector<Luz*> &luz, int num);

    vec3 calcular_color(Rayo rayo, const std::vector<Objeto*>& objetos, std::vector<Luz*> luces, int prof);

    vec3 refract(vec3 &I, vec3 &N, float &ior);

    void fresnel(vec3 &I, vec3 &N, float &ior, float &kr);
};


#endif //CG2023_CAMARA_H
