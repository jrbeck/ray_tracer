#pragma once

#include "Material.h"
#include "Ray3.h"
#include "Intersection.h"

class Shape {
public:
  Shape() : mMaterial(nullptr) {};
  Shape(Material* material) : mMaterial(material) {};

  virtual ~Shape() {};

  Material* mMaterial;

  virtual bool getIntersection(const Ray3* ray, Intersection* intersection) = 0;
};
