#pragma once

#include "ImageBuffer.h"
#include "math/PseudoRandom.h"

class RayTracer {
public:
  RayTracer(int width, int height);
  ~RayTracer();

  // void perform(int workUnits, int distance);
  // unsigned performUnit(int iterations, int distance);
  void printResults();
  void saveOutput();

  ImageBuffer* getOutput();

private:
  // PseudoRandom mPrng;
  ImageBuffer* mOutput;

  unsigned mWidth, mHeight;
};
