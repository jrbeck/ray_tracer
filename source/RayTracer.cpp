#include "RayTracer.h"

RayTracer::RayTracer(int width, int height) :
  mOutput(nullptr),
  mWidth(width),
  mHeight(height)
{
  mOutput = new ImageBuffer();
  mOutput->resize(mWidth, mHeight);

  Vec3 position(2.0, 2.0, 0.0), target(0.2, 0.0, 0.0), up(0.0, 1.0, 0.0);
  VEC3_DATA_TYPE fieldOfView = M_PI * 0.5;
  VEC3_DATA_TYPE aspectRatio = (VEC3_DATA_TYPE)mWidth / (VEC3_DATA_TYPE)mHeight;
  mCamera = new Camera3(position, target, up, fieldOfView, aspectRatio);

  mScene = new Scene();
  mScene->addShape(new Sphere(Vec3(0.0, 0.0, 0.0), 1.0));
  mScene->addShape(new Sphere(Vec3(1.0, 0.0, 0.0), 0.5));
  mScene->addShape(new Sphere(Vec3(1.0, 1.0, 0.0), 0.75));

  mAngle = 0.0;
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
  if (mScene != nullptr) {
    delete mScene;
    mScene = nullptr;
  }
}

void RayTracer::drawFrame() const {
  mAngle += 0.1;
  mCamera->mPosition.x = 5.0 * cos(mAngle);
  mCamera->mPosition.z = 5.0 * sin(mAngle);
  Vec3 viewport[4];
  mCamera->viewport(viewport);

  Vec3 leftEdge, rightEdge;
  Vec3 pixelPosition;
  Ray3 ray = Ray3(mCamera->mPosition, Vec3(1.0, 0.0, 0.0));

  Vec3 lightSource = Vec3(5.0, 10.0, 5.0);
  Vec3 lightAngle;
  VEC3_DATA_TYPE lightStrength;

  Intersection intersection, tempIntersection;
  VEC3_DATA_TYPE intersectionDistance, tempIntersectionDistance;
  bool intersectionResult, tempIntersectionResult;

  for (unsigned j = 0; j < mHeight; j++) {
    leftEdge = Vec3::lerp(viewport[ViewportCorners::topLeft], viewport[ViewportCorners::bottomLeft], (double)j / ((double)(mHeight - 1.0)));
    rightEdge = Vec3::lerp(viewport[ViewportCorners::topRight], viewport[ViewportCorners::bottomRight], (double)j / ((double)(mHeight - 1.0)));
    for (unsigned i = 0; i <  mWidth; i++) {
      pixelPosition = Vec3::lerp(leftEdge, rightEdge, (double)i / ((double)(mWidth - 1.0)));
      ray.mDirection = (pixelPosition - mCamera->mPosition).unit();

      intersectionResult = false;
      intersectionDistance = 1000000.0;
      for (size_t i = 0; i < mScene->mShapes.size(); ++i) {
        tempIntersectionResult = mScene->mShapes[i]->getIntersection(&ray, &tempIntersection);
        if (tempIntersectionResult) {
          tempIntersectionDistance = Vec3::dist(ray.mOrigin, tempIntersection.mPosition);
          if (tempIntersectionDistance < intersectionDistance) {
            intersectionDistance = tempIntersectionDistance;
            intersectionResult = true;
            intersection = tempIntersection;
          }
        }
      }

      if (intersectionResult) {
        lightAngle = (lightSource - intersection.mPosition).unit();
        lightStrength = fmax(0.0, lightAngle * intersection.mNormal);
        mOutput->setRgb(i, j,
          255.0 * lightStrength,
          255.0 * lightStrength,
          255.0 * lightStrength
        );
      }
      else {
        mOutput->setRgb(i, j, 0, 0, 127);
      }
    }
  }
}

void RayTracer::saveOutput() {
  mOutput->savePng("output_001.png");
}

ImageBuffer* RayTracer::getOutput() {
  return mOutput;
}
