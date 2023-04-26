//
// Created by hgallegos on 30/03/2023.
//

#include "Camara.h"

void Camara::configurar(float _near, float fov, int ancho, int alto,
                vec3 pos_eye, vec3 center, vec3 up) {
    f = _near;
    w = float(ancho);
    h = float(alto);
    a = 2.0 * f * tan(fov * M_PI/360);
    b = w / h * a;
    eye = pos_eye;
    ze = eye - center;
    ze.normalize();
    xe = up.cruz(ze);
    xe.normalize();
    ye = ze.cruz(xe);
}

void Camara::renderizar(std::vector<Objeto *> &objetos,  std::vector<Luz*> &luces, int num) {
    Rayo rayo;
    rayo.ori = eye;
    vec3 dir;

    pImg = new CImg<BYTE>(w, h, 1, 10);
    CImgDisplay dis_img((*pImg), "Esto no es pain.exe");

    vec3 color;
    for(int x=0;  x < w; x++) {
        for (int y = 0; y < h; y++) {
            dir = ze * (-f) + ye * a * (y / h - 0.5f) + xe * b * (x / w - 0.5f);
            dir.normalize();
            rayo.dir = dir;

            color = calcular_color(rayo, objetos, luces, 1);

            (*pImg)(x,h-1-y,0) = (BYTE)(color.x * 255);
            (*pImg)(x,h-1-y,1) = (BYTE)(color.y * 255);
            (*pImg)(x,h-1-y,2) = (BYTE)(color.z * 255);
        }
    }
    dis_img.render((*pImg));
    dis_img.paint();
    std::string nombre_archivo = "imagen" + std::to_string(num) + ".bmp";
    pImg->save(nombre_archivo.c_str());
/*    while (!dis_img.is_closed()) {
        dis_img.wait();
    }
*/
    delete pImg;
    pImg = nullptr;
}

vec3 Camara::calcular_color(Rayo rayo, const std::vector<Objeto*>& objetos, std::vector<Luz*> luces, int prof) {
    vec3 color, normal,normal_tmp;
    color.set(0,0,0);
    bool is_intersect = false;
    float distance = FLT_MAX;
    float distance_tmp = 0;
    Objeto * pObjeto = nullptr;

    for (auto obj : objetos){
        if (obj->intersectar(rayo, distance_tmp, normal_tmp)){
            is_intersect = true;
            if (distance_tmp < distance){
                distance = distance_tmp;
                normal = normal_tmp;
                pObjeto = obj;
            }
        }
    }

    for (auto luz: luces) {
        if (is_intersect){
            vec3 pi = rayo.ori + rayo.dir * distance;
            vec3 L = luz->pos - pi;
            L.normalize();
            vec3 luz_ambiente = vec3(1,1,1) * 0.2;

            bool is_shadow = false;
            Rayo shadow;
            shadow.ori = pi + normal*0.0005;
            shadow.dir = L;

            for (auto obj : objetos){
                if (obj->intersectar(shadow, distance_tmp, normal_tmp)){
                    is_shadow = true;
                }
            }

            // evaluar por sombras
            if (!is_shadow){
                vec3 luz_difusa = vec3(0,0,0);
                float factor_difuso = normal.punto(L);

                if (factor_difuso > 0 and not pObjeto->es_luz)
                    luz_difusa = luz->color * pObjeto->k_difusa * factor_difuso;

                vec3 r =  (normal * 2 * (L.punto(normal))) - L ;
                vec3 V = -rayo.dir;
                r.normalize();
                float factor_especular = r.punto(rayo.dir*(-1));
                vec3 luz_especular = vec3(0,0,0);

                if (factor_especular > 0 and not pObjeto->es_luz)
                    luz_especular = luz->color * pObjeto->k_especular * pow(factor_especular, pObjeto->n);

                color = color + pObjeto->color * (luz_ambiente + luz_difusa + luz_especular);

                if (pObjeto->es_transparente){
// kr kt
                    vec3 refractionColor(0,0,0);
                    // compute fresnel
                    float kr;
                    fresnel(rayo.dir, normal, pObjeto->index_refraction, kr);
                    bool outside = rayo.dir.punto(normal) < 0;
                    vec3 bias = 0.0005 * normal;
                    // compute refraction if it is not a case of total internal reflection
                    if (kr < 1) {
                        vec3 refractionDirection = refract(rayo.dir, normal, pObjeto->index_refraction);
                        refractionDirection.normalize();
                        vec3 refractionRayOrig = outside ? pi - bias : pi + bias;
                        Rayo rayo_refraccion(refractionRayOrig, refractionDirection);
                        refractionColor = calcular_color(rayo_refraccion, objetos, luces, prof + 1);
                    }

                    vec3 reflectionDirection = 2 * (V.punto(normal)) * normal - V;// reflect(dir, normal).normalize();
                    reflectionDirection.normalize();
                    vec3 reflectionRayOrig = outside ? pi + bias : pi - bias;
                    Rayo rayo_reflexivo;
                    rayo_reflexivo.ori = reflectionRayOrig;
                    rayo_reflexivo.dir = reflectionDirection;
                    vec3 reflectionColor = calcular_color(rayo_reflexivo, objetos, luces, prof + 1);

                    // mix the two
                    color = color + reflectionColor * kr + refractionColor * (1 - kr);
                }else{
                    if (pObjeto->k_espejo > 0 and prof + 1 <= prof_max) {
                        // rayos reflexivos
                        Rayo rayo_reflexivo;
                        rayo_reflexivo.ori = pi + 0.0005 * normal;
                        rayo_reflexivo.dir = 2 * (V.punto(normal)) * normal - V;
                        rayo_reflexivo.dir.normalize();
                        vec3 color_reflexivo = calcular_color(rayo_reflexivo, objetos, luces, prof + 1);
                        color = color + pObjeto->k_espejo * color_reflexivo;
                    }
                }
                color.max_to_one();
            }else{
                color = color + pObjeto->color * (luz_ambiente);
            }
            color.max_to_one();

        }else{
            color.set(0,0,0);
        }
    }
    pObjeto = nullptr;
    return color;
}

vec3 Camara::refract(vec3 &I, vec3 &N, float &ior)
{
    float cosi = std::clamp(-1.0f, 1.0f, I.punto(N));
    float etai = 1, etat = ior;
    vec3 n = N;
    if (cosi < 0) { cosi = -cosi; } else { std::swap(etai, etat); n= -N; }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? vec3(0,0,0) : eta * I + (eta * cosi - sqrtf(k)) * n;
}

void Camara::fresnel(vec3 &I, vec3 &N, float &ior, float &kr)
{
    float cosi = std::clamp(-1.0f, 1.0f, I.punto(N));
    float etai = 1, etat = ior;
    if (cosi > 0) { std::swap(etai, etat); }
    // Compute sini using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
    // Total internal reflection
    if (sint >= 1) {
        kr = 1;
    }
    else {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
    }
    // As a consequence of the conservation of energy, the transmittance is given by:
    // kt = 1 - kr;
}