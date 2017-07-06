#pragma once

#include <vector>

#include "math/Shape.h"

class Scene {
public:
  std::vector<Shape*> mShapes;

  Scene();
  ~Scene();

  void addShape(Shape* shape);
};
