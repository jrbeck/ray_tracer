#!/bin/sh
clear
g++ \
  source/Tracer.cpp \
  source/SdlApp.cpp \
  source/ImageBuffer.cpp \
  source/Pixel.cpp \
  source/RayTracer.cpp \
  source/Camera3.cpp \
  source/Scene.cpp \
  source/math/Vec3.cpp \
  source/math/Ray3.cpp \
  source/math/Light.cpp \
  source/math/Sphere.cpp \
  source/math/Intersection.cpp \
  source/math/PseudoRandom.cpp \
  source/lib/lodepng.cpp \
  -lSDL2 -ansi -pedantic -Wall -Wextra -O3 -std=c++11 -o tracer
