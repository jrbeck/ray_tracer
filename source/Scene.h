#pragma once

#include <vector>

#include "math/Light.h"
#include "math/Shape.h"

class Scene {
public:
  std::vector<Light*> mLights;
  std::vector<Shape*> mShapes;

  Scene();
  ~Scene();

  void addLight(Light* light);
  void addShape(Shape* shape);
};
