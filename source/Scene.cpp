#include "Scene.h"

Scene::Scene() {}

Scene::~Scene() {
  for (size_t i = 0; i < mLights.size(); ++i) {
    if (mLights[i] != nullptr) {
      delete mLights[i];
      mLights[i] = nullptr;
    }
  }

  for (size_t i = 0; i < mMaterials.size(); ++i) {
    if (mMaterials[i] != nullptr) {
      delete mMaterials[i];
      mMaterials[i] = nullptr;
    }
  }

  for (size_t i = 0; i < mShapes.size(); ++i) {
    if (mShapes[i] != nullptr) {
      delete mShapes[i];
      mShapes[i] = nullptr;
    }
  }
}

bool Scene::getIntersection(const Ray3& ray, Intersection& intersection) {
  Intersection tempIntersection;
  bool intersectionResult = false;
  VEC3_DATA_TYPE intersectionDistance, tempIntersectionDistance;
  intersectionDistance = 1000000.0;

  for (size_t shapeIndex = 0; shapeIndex < mShapes.size(); ++shapeIndex) {
    if (!mShapes[shapeIndex]->getIntersection(&ray, &tempIntersection)) {
      continue;
    }
    tempIntersectionDistance = Vec3::dist(ray.mOrigin, tempIntersection.mPosition);
    if (tempIntersectionDistance < intersectionDistance) {
      intersectionDistance = tempIntersectionDistance;
      intersectionResult = true;
      intersection = tempIntersection;
    }
  }

  return intersectionResult;
}

void Scene::addLight(Light* light) {
  mLights.push_back(light);
}

void Scene::addMaterial(Material* material) {
  mMaterials.push_back(material);
}

void Scene::addShape(Shape* shape) {
  mShapes.push_back(shape);
}
