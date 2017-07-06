#pragma once

#include "Ray3.h"
#include "Intersection.h"

class Shape {
public:
  virtual bool getIntersection(const Ray3* ray, Intersection* intersection) = 0;
};
