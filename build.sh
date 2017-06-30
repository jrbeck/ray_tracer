#!/bin/sh
clear
g++ source/Tracer.cpp source/SdlApp.cpp source/ImageBuffer.cpp source/Pixel.cpp source/RayTracer.cpp source/math/PseudoRandom.cpp source/math/Vec3.cpp source/lib/lodepng.cpp source/Camera3.cpp -lSDL2 -ansi -pedantic -Wall -Wextra -O3 -o tracer
