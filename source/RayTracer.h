#pragma once

#include "ImageBuffer.h"
#include "Scene.h"
#include "math/Vec3.h"
#include "math/Ray3.h"
#include "math/Sphere.h"
#include "math/Intersection.h"
#include "Camera3.h"
#include "math/PseudoRandom.h"

#define AMBIENT_LIGHT_STRENGTH (0.1)

class RayTracer {
public:
  RayTracer(int width, int height);
  ~RayTracer();

  void drawFrame() const;
  void saveOutput();

  ImageBuffer* getOutput();

private:
  Vec3 traceRay(const Ray3& ray) const;

  unsigned mWidth, mHeight;
  mutable VEC3_DATA_TYPE mAngle;
  ImageBuffer* mOutput;

  Camera3* mCamera;
  Scene* mScene;
};
