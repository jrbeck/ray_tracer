#pragma once

#include <thread>

#include "ImageBuffer.h"
#include "Scene.h"
#include "math/Vec3.h"
#include "math/Ray3.h"
#include "math/Sphere.h"
#include "math/Mesh.h"
#include "math/Intersection.h"
#include "Camera3.h"
#include "math/PseudoRandom.h"

#define AMBIENT_LIGHT_STRENGTH (0.1)

enum RayThreadStates {
  dormant = 0,
  working = 1,
  complete = 2
};


struct RayThread {
  bool active;
  int state;
  std::thread thread;
  int j;
  Vec3 leftEdge;
  Vec3 rightEdge;
};


class RayTracer {
public:
  RayTracer(int width, int height, Scene* scene);
  ~RayTracer();

  void drawFrame() const;
  void saveOutput();

  ImageBuffer* getOutput();

private:
  void drawScanline(RayThread* rayThread) const;
  Vec3 traceRay(const Ray3& ray, int bounce) const;

  const size_t kNumThreads = 8;
  const size_t kMaxBounces = 2;

  unsigned mWidth, mHeight;
  mutable VEC3_DATA_TYPE mAngle;
  ImageBuffer* mOutput;
  RayThread* mRayThreads;

  Camera3* mCamera;
  Scene* mScene;
};
