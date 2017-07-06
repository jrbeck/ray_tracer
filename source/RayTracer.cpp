#include "RayTracer.h"

RayTracer::RayTracer(int width, int height) :
  mOutput(nullptr),
  mWidth(width),
  mHeight(height)
{
  mOutput = new ImageBuffer();
  mOutput->resize(mWidth, mHeight);

  // for (int j = 0; j < mHeight; j++) {
  //   for (int i = 0; i < mWidth; i++) {
  //     mOutput->setRgb(i, j, i, j, i);
  //   }
  // }

  Vec3 position(0.0f, 0.0f, 0.0f), target(1.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f);
  CAMERA_FLOAT_T fieldOfView = M_PI * 0.5;
  CAMERA_FLOAT_T aspectRatio = 1.0;
  mCamera = new Camera3(position, target, up, fieldOfView, aspectRatio);
}

RayTracer::~RayTracer() {
  if (mOutput != nullptr) {
    delete mOutput;
    mOutput = nullptr;
  }
  if (mCamera != nullptr) {
    delete mCamera;
    mCamera = nullptr;
  }
}

void RayTracer::drawFrame() const {
  Vec3 viewport[4];
  mCamera->viewport(viewport);
  // for (int i = 0; i < 4; i++) {
  //   viewport[i].print();
  // }

  Vec3 leftEdge, rightEdge, pixelPosition;
  Ray3 ray = Ray3(mCamera->mPosition, Vec3(1.0, 0.0, 0.0));
  Sphere sphere = Sphere(Vec3(5.0, 0.0, 0.0), 1.0);
  Intersection intersection;
  bool intersectionResult;

  for (int j = 0; j < mHeight; j++) {
    leftEdge = Vec3::lerp(viewport[ViewportCorners::topLeft], viewport[ViewportCorners::bottomLeft], (double)j / ((double)(mHeight - 1.0)));
    rightEdge = Vec3::lerp(viewport[ViewportCorners::topRight], viewport[ViewportCorners::bottomRight], (double)j / ((double)(mHeight - 1.0)));
    for (int i = 0; i <  mWidth; i++) {
      pixelPosition = Vec3::lerp(leftEdge, rightEdge, (double)i / ((double)(mWidth - 1.0)));
      // printf("(%.2f,%.2f,%.2f)", pixelPosition.x, pixelPosition.y, pixelPosition.z);


      ray.mDirection = (pixelPosition - mCamera->mPosition).unit();
      intersectionResult = sphere.getIntersection(&ray, &intersection);
      // printf("%d\n", intersectionResult);
      if (intersectionResult) {
        // intersection.mPosition.print("pos");
        // intersection.mNormal.print("norm");
        mOutput->setRgb(i, j, 255, 255, 255);
      }
      else {
        mOutput->setRgb(i, j, 0, 0, 127);
      }


    }
    // printf("\n");
  }
}

void RayTracer::saveOutput() {
  mOutput->savePng("output_001.png");
}

ImageBuffer* RayTracer::getOutput() {
  return mOutput;
}
