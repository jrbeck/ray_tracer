#include "RayTracer.h"

RayTracer::RayTracer(int width, int height) :
  mOutput(nullptr),
  mWidth(width),
  mHeight(height)
{
  mOutput = new ImageBuffer();
  mOutput->resize(mWidth, mHeight);

  for (int j = 0; j < 512; j++) {
    for (int i = 0; i < 256; i++) {
      mOutput->setRgb(i, j, i, j, i);
    }
  }
}

RayTracer::~RayTracer() {
  if (mOutput != nullptr) {
    delete mOutput;
    mOutput = nullptr;
  }
}

void RayTracer::saveOutput() {
  mOutput->savePng("output_001.png");
}

ImageBuffer* RayTracer::getOutput() {
  return mOutput;
}
