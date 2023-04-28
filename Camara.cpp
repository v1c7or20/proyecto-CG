#include "Camara.h"

using namespace std;
void Camara::configurar(float _near, float fov, int ancho, int alto,
                        vec3 pos_eye, vec3 center, vec3 up) {
    f = _near;
    w = ancho;
    h = alto;
    a = 2 * f * tan(fov * M_PI/360);
    b = w / h * a;
    eye = pos_eye;
    ze = eye - center;
    ze.normalize();
    xe = up.cruz(ze);
    xe.normalize();
    ye = ze.cruz(xe);
}

vec3 Camara::calcular_color(Rayo rayo, const std::vector<Objeto*>& objetos, std::vector<Luz*> luces, int prof) {
    vec3 color(0,0,0); // color de fondo;
    vec3 normal, normal_tmp;
    Objeto *pObjeto;
    bool hay_interseccion = false;
    float t_tmp, t = FLT_MAX;
    for(auto pObj : objetos) {
        if ( pObj->intersectar(rayo, t_tmp, normal_tmp)) {
            hay_interseccion = true;
            if (t_tmp < t) {
                t = t_tmp;
                normal = normal_tmp;
                pObjeto = pObj;
            }
        }
    }
    if (hay_interseccion and pObjeto->es_luz){
        color = pObjeto->color;
    } else if ( hay_interseccion ) {
        vec3 pi = rayo.ori + rayo.dir * t;
        vec3 V = -rayo.dir;
        if (pObjeto->es_transparente) {
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
            Rayo rayo_reflexivo(reflectionRayOrig, reflectionDirection);
            vec3 reflectionColor = calcular_color(rayo_reflexivo, objetos, luces, prof + 1);

            // mix the two
            color = reflectionColor * kr + refractionColor * (1 - kr);
        } else {
            if (pObjeto->k_espejo > 0 and  prof + 1 <= prof_max) {
                // rayos reflexivos
                Rayo rayo_reflexivo;
                rayo_reflexivo.ori = pi + 0.0005 * normal;
                rayo_reflexivo.dir = 2 * (V.punto(normal)) * normal - V;
                rayo_reflexivo.dir.normalize();
                vec3 color_reflexivo = calcular_color(rayo_reflexivo, objetos, luces, prof + 1);
                color = pObjeto->k_espejo * color_reflexivo;
            }
        }

        vec3 luz_total =vec3(1, 1, 1) * 0.4;

        for (auto luz: luces) {
            vec3 L = luz->pos - pi;
            float distancia = L.modulo();
            L.normalize();
            // evaluar si hay sombra
            bool esta_sombra = false;
            Rayo rayo_sombra(pi + 0.0005*normal, L);
            for(auto pObj : objetos) {
                if ((not pObj->es_luz) and (not pObj->es_transparente) and
                    pObj->intersectar(rayo_sombra, t_tmp, normal_tmp) and
                    t_tmp < distancia) {
                    esta_sombra = true;
                }
            }

            if (!esta_sombra) {
                vec3 luz_difusa = vec3(0, 0, 0);
                float factor_difuso = normal.punto(L);
                if (factor_difuso > 0 and not pObjeto->es_luz)
                    luz_difusa = luz->color * pObjeto->k_difusa * factor_difuso;

                vec3 luz_especular = vec3(0, 0, 0);
                vec3 R = 2 * (L.punto(normal)) * normal - L;
                float factor_especular = R.punto(V);
                if (factor_especular > 0 and not pObjeto->es_luz)
                    luz_especular = luz->color * pObjeto->k_especular * pow(factor_especular, pObjeto->n);
                luz_total = (luz_total + luz_difusa + luz_especular) / log(distancia);
            } else {
            }
        }
        color = color + pObjeto->color * luz_total ;
        color.max_to_one();
    }
    return color;
}

vec3 Camara::refract(vec3 &I, vec3 &N, float &ior)
{
    float cosi = clamp(-1.0f, 1.0f, I.punto(N));
    float etai = 1, etat = ior;
    vec3 n = N;
    if (cosi < 0) { cosi = -cosi; } else { std::swap(etai, etat); n= -N; }
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? vec3(0,0,0) : eta * I + (eta * cosi - sqrtf(k)) * n;
}

void Camara::fresnel(vec3 &I, vec3 &N, float &ior, float &kr)
{
    float cosi = clamp(-1.0f, 1.0f, I.punto(N));
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
void Camara::renderizar(std::vector<Objeto *> &objetos, std::vector<Luz*> &luces, int num) {
    pImg = new CImg<BYTE>(w, h, 1, 10);
    CImgDisplay dis_img((*pImg), "Esto no es paint.exe");
    Rayo rayo(eye);
    vec3 color, dir;
    for(int x=0;  x < w; x++) {
        for (int y = 0; y < h; y++) {
            dir = ze * (-f) + ye * a * (y / h - 0.5) + xe * b * (x / w - 0.5);
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
    string nombre_archivo = "imagen" + to_string(num) + ".bmp";
    pImg->save(nombre_archivo.c_str());
    /*while (!dis_img.is_closed()) {
        dis_img.wait();
    }*/
}