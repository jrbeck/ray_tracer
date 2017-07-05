#pragma once

#include "math/Ray3.h"

class Shape {
public:
  virtual void getIntersection(const Ray3* ray, Intersection* intersection) = 0;
}
