//
// Created by hgallegos on 30/03/2023.
//

#ifndef CG2023_VEC3_H
#define CG2023_VEC3_H
#include <cmath>

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

class vec3 {
public:
    float x, y, z;
    vec3() {}
    void set(float _x, float _y, float _z) {
        x = _x;
        y = _y;
        z = _z;
    }
    vec3(float _x, float _y, float _z):x{_x}, y{_y}, z{_z}{}
    vec3 cruz(vec3 v) { return vec3(y*v.z - v.y*z, v.x*z - x*v.z, x*v.y - v.x*y); }
    float punto(vec3 v) { return x*v.x + y*v.y + z*v.z; }
    vec3 operator+(vec3 v) { return vec3(x + v.x, y +v.y, z+v.z); }
    vec3 operator*(vec3 v) { return vec3(x * v.x, y *v.y, z*v.z); }
    vec3 operator-(vec3 v) { return vec3(x - v.x, y -v.y, z-v.z); }
    vec3 operator-() { return vec3(-x,-y,-z); }
    vec3 operator*(float f) { return vec3(f*x, f*y, f*z); }
    vec3 operator/(float f) { return vec3(x/f, y/f, z/f); }

    void operator*=(vec3 v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
    }

    void operator+=(vec3 v) {
        x += v.x;
        y += v.y;
        z += v.z;
    }
    void normalize() {
        double m = modulo();
        x = x/m;
        y = y/m;
        z = z/m;
    }
    double modulo() {
        return sqrt(x*x + y*y + z*z);
    }
    void max_to_one() {
        float max_value = std::max(x, std::max(y,z));
        if (max_value > 1.0){
            x = x / max_value;
            y = y / max_value;
            z = z / max_value;
        }
    }
};

vec3 operator*(float f, vec3 v);

#endif //CG2023_VEC3_H
