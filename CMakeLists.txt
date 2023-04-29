cmake_minimum_required(VERSION 3.19)
project(CG2023)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(CG2023 main.cpp vec3.h Rayo.h
        Objeto.h Camara.cpp Camara.h Esfera.h Luz.h Plano.h Cubo.h vec3.cpp Cilindro.h Firefly.h)