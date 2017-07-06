#include "Scene.h"

Scene::Scene() {}

Scene::~Scene() {
  for (size_t i = 0; i < mShapes.size(); ++i) {
    delete mShapes[i];
    mShapes[i] = nullptr;
  }
}

void Scene::addShape(Shape* shape) {
  mShapes.push_back(shape);
}
