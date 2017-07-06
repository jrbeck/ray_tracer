#pragma once

#include "Vec3.h"
#include "Shape.h"

class Sphere : public Shape {
public:
  Vec3 mCenter;
  VEC3_DATA_TYPE mRadius;

  Sphere(Vec3 center, VEC3_DATA_TYPE radius);

  bool getIntersection(const Ray3* ray, Intersection* intersection);
};
