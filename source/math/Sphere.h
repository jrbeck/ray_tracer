#pragma once

#include "math/Vec3.h"
#include "math/Shape.h"

class Sphere : public Shape {
public:
  VEC3_DATA_TYPE mRadius;

  Sphere(Vec3 center, VEC3_DATA_TYPE radius);

  void getIntersection(const Ray3* ray, Intersection* intersection);
};
