#include <iostream>
#include "Camara.h"

int main() {
    srand( (unsigned)time( nullptr ) );

    std::vector<Objeto *>objetos;
    Objeto *p1 = nullptr;
    p1 = new Esfera(vec3(10,0,0), 8, vec3(0,0,1), 0.7f);
    p1->k_espejo = 1;
    objetos.emplace_back(p1);
    p1 = new Cubo(vec3(10,10,10), vec3(14,14,14), vec3(0,1,0));
    p1->k_espejo = 1;
    objetos.push_back(p1);
    p1 = new Esfera(vec3(0,10,0), 8, vec3(1,0,0), 0.6);
    p1->k_espejo = 1;
    objetos.push_back(p1);
    p1 = new Plano(vec3(0,1,0), 1, vec3(0.123, 0.456, 0.789));
    p1->es_transparente = true;
    p1->k_espejo = 0.8;
    p1->setConstantes(0.6, 0.4, 32);

    objetos.push_back(p1);
    p1 = new Cilindro(vec3(-20,0,0), vec3(-10,10,0),5, vec3(0,1,1));
    p1->setConstantes(0.7,0.3,8);
    objetos.emplace_back(p1);
    /*
    for (int i = 1; i < 2; i++ ){
        for (int j = 1; j < 2; ++j) {
            for (int k = 1; k < 2; ++k) {
                std::cout<<(float) rand()/RAND_MAX<<std::endl;
                objetos.push_back(new Esfera(vec3(20*(float) rand()/RAND_MAX,20*(float) rand()/RAND_MAX,50*((float) rand()/RAND_MAX)), (float) rand()/RAND_MAX*3,
                                             vec3((float) rand()/RAND_MAX,(float) rand()/RAND_MAX,(float) rand()/RAND_MAX), (float) rand()/RAND_MAX));
            }
        }
    }
*/
    std::vector<Luz*> luces;
    Luz luz(vec3(30,30,30), vec3(1,1,1));
    luces.emplace_back(&luz);

    Camara cam;
    for (int x = 1, n=1; x < 11; x++, n++){
        cam.configurar(3,60,600,800,
                       vec3(float(x),5,50),
                       vec3(0,0,0),
                       vec3(0,1,0));
        cam.renderizar(objetos, luces, n);
    }
    return 0;
}
