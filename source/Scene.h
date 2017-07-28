#pragma once

#include <vector>

#include "math/Light.h"
#include "math/Material.h"
#include "math/Shape.h"

class Scene {
public:
  std::vector<Light*> mLights;
  std::vector<Material*> mMaterials;
  std::vector<Shape*> mShapes;

  Scene();
  ~Scene();

  bool getIntersection(const Ray3& ray, Intersection& intersection);

  void addLight(Light* light);
  void addMaterial(Material* material);
  void addShape(Shape* shape);
};
