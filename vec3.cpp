//
// Created by LAPTOP1 on 19/4/2023.
//
#include "vec3.h"

vec3 operator*(float f, vec3 v) { return {v.x * f, v.y * f, v.z*f}; }