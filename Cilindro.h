//
// Created by LAPTOP1 on 20/4/2023.
//

#ifndef CG2023_CILINDRO_H
#define CG2023_CILINDRO_H
#include "Objeto.h"


class Cilindro: public Objeto{
public:
    vec3 pa, pb;
    float ra;

    Cilindro(vec3 _pa, vec3 _pb, float _ra, vec3 _color):
            pa{_pa}, pb{_pb}, ra{_ra}, Objeto(_color, 1){}

    bool intersectar(Rayo rayo, float &t, vec3 &normal) override{
        vec3 ro = rayo.ori;
        vec3 rd = rayo.dir;
        vec3 ca = pb-pa;
        vec3 oc = ro-pa;
        float caca = ca.punto(ca);
        float card = ca.punto(rd);
        float caoc = ca.punto(oc);
        float a = caca - card*card;
        float b = caca * oc.punto(rd) - caoc*card;
        float c = caca * oc.punto(oc) - caoc*caoc - ra*ra*caca;
        float h = b*b - a*c;
        if( h < 0.0 ) return false; //no intersection
        h = sqrt(h);
        t = (-b-h)/a;
        //if (t <= 0) return false;
        // body
        float y = caoc + t*card;
        if ( y > 0.0 && y < caca && t > 0) {
            normal = (oc + t*rd - ca*y/caca)/ra;
            normal.normalize();
            return true;
        }
        // caps
        t = (((y<0.0)?0.0:caca) - caoc)/card;
        if (t <= 0) return false;
        if( abs(b+a*t)<h ) {
            normal =  ca * sgn(y) / caca;
            normal.normalize();
            return true;
        }
        return false; //no intersection
    }

    void moveUp(float d) override {

    }

    void moveDown(float d) override {

    }

    void moveLeft() override {

    }

    void moveRight() override {

    }
};

#endif //CG2023_CILINDRO_H
