#pragma once

#include "ImageBuffer.h"
#include "math/Vec3.h"
#include "math/Ray3.h"
#include "math/Sphere.h"
#include "math/Intersection.h"
#include "Camera3.h"

class RayTracer {
public:
  RayTracer(int width, int height);
  ~RayTracer();

  void drawFrame() const;
  void saveOutput();

  ImageBuffer* getOutput();

private:
  unsigned mWidth, mHeight;
  ImageBuffer* mOutput;

  Camera3* mCamera;
};
