#include <iostream>
#include "Camara.h"
#include "firefly.h"

int main() {
    srand( (unsigned)time( nullptr ) );

    std::vector<Objeto *>objetos;
    Objeto *p1 = nullptr;
    p1 = new Plano(vec3(0,1,0), 2, vec3(0.41, 0.71, 0.61));
    p1->k_espejo = 0.8;
    p1->setConstantes(0.6, 0.4, 32);
    objetos.push_back(p1);
    p1 = new Cilindro(vec3(0,0,0), vec3(0,30,0),10, vec3(1,1,1));
    p1->es_transparente = true;
    p1->setConstantes(0.7,0.3,8);
    p1->index_refraction = 1.5f;
    objetos.emplace_back(p1);
    p1 = new Esfera(vec3(0,3,0), 1, vec3(1, 0.968, 0), 0.6);
    p1->es_luz=true;
    p1->es_transparente = true;
    objetos.emplace_back(p1);

    std::vector<Luz*> luces;
    Luz *luz = new Luz(vec3(50,40,30), vec3(1,1,1));
    luces.emplace_back(luz);
    luz = new Luz(vec3(0,3,0), vec3(1, 0.968, 0));
    luces.emplace_back(luz);

    firefly fly1;
    fly1.luz = luces.at(1);
    fly1.cuerpo = (Esfera *)objetos.at(2);

    Camara cam;
    bool ret1=false;
    for (int x = 1, n=1; x <= 240; x++, n++){
        cam.configurar(3,60,600,800,
                       vec3(1+float(x)/4,25 + float(x)/10,80),
                       vec3(0,0,0),
                       vec3(0,1,0));
        fly1.moveUp(1);
        cam.renderizar(objetos, luces, n);
    }
    return 0;
}
