//
// Created by LAPTOP1 on 25/4/2023.
//

#ifndef CG2023_FIREFLY_H
#define CG2023_FIREFLY_H
#include "Esfera.h"
#include "Luz.h"

class Firefly{
public:
    Esfera * cuerpo;
    Luz * luz;

    void moveUp(float step){
        cuerpo->centro.y+=step;
        luz->pos.y+=step;
    }

    void moveDown(float step){
        cuerpo->centro.y-=step;
        luz->pos.y-=step;
    }

    void moveLeft(float step){
        cuerpo->centro.x-=step;
        luz->pos.x-=step;
    }

    void moveRight(float step){
         cuerpo->centro.x+=step;
         luz->pos.x+=step;
    }

    void moveForward(float step){
        cuerpo->centro.z+=step;
        luz->pos.z+=step;
    }

    void moveBackward(float step){
        cuerpo->centro.z-=step;
        luz->pos.z-=step;
    }

    void upDown(float step, int i){
        if (i/20 % 2 == 0){
            moveUp(step);
        }else{
            moveDown(step);
        }
    }
    void downUp(float step, int i){
        if (i/20 % 2 == 1){
            moveUp(step);
        }else{
            moveDown(step);
        }
    }

};

#endif //CG2023_FIREFLY_H
