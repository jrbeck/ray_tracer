#include "RayTracer.h"

RayTracer::RayTracer(int width, int height) :
  mOutput(nullptr),
  mWidth(width),
  mHeight(height)
{
  mOutput = new ImageBuffer();
  mOutput->resize(mWidth, mHeight);

  Vec3 position(2.0, 15.0, 0.0), target(0.2, 0.0, 0.0), up(0.0, 1.0, 0.0);
  VEC3_DATA_TYPE fieldOfView = M_PI * 0.5;
  VEC3_DATA_TYPE aspectRatio = (VEC3_DATA_TYPE)mWidth / (VEC3_DATA_TYPE)mHeight;
  mCamera = new Camera3(position, target, up, fieldOfView, aspectRatio);

  mScene = new Scene();
  mScene->addLight(new Light(Vec3(0.0, 15.0, 5.0), Vec3(1.0, 0.0, 0.0)));
  mScene->addLight(new Light(Vec3(0.0, 0.0, 5.0), Vec3(0.0, 1.0, 1.0)));

  mScene->addShape(new Sphere(Vec3(0.0, 0.0, 0.0), 1.0));
  mScene->addShape(new Sphere(Vec3(1.0, 0.0, 0.0), 0.5));
  mScene->addShape(new Sphere(Vec3(1.0, 1.0, 0.0), 0.75));

  PseudoRandom pseudoRandom = PseudoRandom(5);
  for (int i = 0; i < 10; ++i) {
    VEC3_DATA_TYPE x = pseudoRandom.nextDouble(-5.0, 5.0);
    VEC3_DATA_TYPE y = pseudoRandom.nextDouble(-5.0, 5.0);
    VEC3_DATA_TYPE z = pseudoRandom.nextDouble(-5.0, 5.0);
    VEC3_DATA_TYPE r = pseudoRandom.nextDouble(0.5, 2.0);
    mScene->addShape(new Sphere(Vec3(x, y, z), r));
  }

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
  mCamera->mPosition.x = 15.0 * cos(mAngle);
  mCamera->mPosition.z = 15.0 * sin(mAngle);
  Vec3 viewport[4];
  mCamera->viewport(viewport);

  Vec3 leftEdge, rightEdge;
  Vec3 pixelPosition;
  Ray3 ray = Ray3(mCamera->mPosition, Vec3(1.0, 0.0, 0.0));

  for (unsigned j = 0; j < mHeight; j++) {
    leftEdge = Vec3::lerp(viewport[ViewportCorners::topLeft], viewport[ViewportCorners::bottomLeft], (double)j / ((double)(mHeight - 1.0)));
    rightEdge = Vec3::lerp(viewport[ViewportCorners::topRight], viewport[ViewportCorners::bottomRight], (double)j / ((double)(mHeight - 1.0)));
    // #pragma omp parallel for
    for (unsigned i = 0; i <  mWidth; i++) {
      pixelPosition = Vec3::lerp(leftEdge, rightEdge, (double)i / ((double)(mWidth - 1.0)));
      ray.mDirection = (pixelPosition - mCamera->mPosition).unit();

      Vec3 color = RayTracer::traceRay(ray);
      mOutput->setRgb(i, j,
        255.0 * fmin(color.x, 1.0),
        255.0 * fmin(color.y, 1.0),
        255.0 * fmin(color.z, 1.0)
      );
    }
  }
}

Vec3 RayTracer::traceRay(const Ray3& ray) const {
  Vec3 lightAngle;
  VEC3_DATA_TYPE lightStrength;

  Intersection intersection, tempIntersection;
  VEC3_DATA_TYPE intersectionDistance, tempIntersectionDistance;
  bool intersectionResult, tempIntersectionResult;
  size_t intersectionShapeIndex;

  intersectionResult = false;
  intersectionDistance = 1000000.0;
  for (size_t shapeIndex = 0; shapeIndex < mScene->mShapes.size(); ++shapeIndex) {
    tempIntersectionResult = mScene->mShapes[shapeIndex]->getIntersection(&ray, &tempIntersection);
    if (tempIntersectionResult) {
      tempIntersectionDistance = Vec3::dist(ray.mOrigin, tempIntersection.mPosition);
      if (tempIntersectionDistance < intersectionDistance) {
        intersectionDistance = tempIntersectionDistance;
        intersectionResult = true;
        intersection = tempIntersection;
        intersectionShapeIndex = shapeIndex;
      }
    }
  }

  if (!intersectionResult) {
    return Vec3(0.0, 0.0, 0.0);
  }

  // figure out what color it is
  Vec3 lightAccumulator = Vec3(AMBIENT_LIGHT_STRENGTH, AMBIENT_LIGHT_STRENGTH, AMBIENT_LIGHT_STRENGTH);

  for (size_t lightIndex = 0; lightIndex < mScene->mLights.size(); ++lightIndex) {
    Light* light = mScene->mLights[lightIndex];
    bool shadowIntersectionResult = false;
    Ray3 shadowRay = Ray3(intersection.mPosition, (light->mPosition - intersection.mPosition).unit());
    for (size_t shapeIndex = 0; shapeIndex < mScene->mShapes.size() && !shadowIntersectionResult; ++shapeIndex) {
      if (shapeIndex == intersectionShapeIndex) {
        continue;
      }
      shadowIntersectionResult = shadowIntersectionResult || mScene->mShapes[shapeIndex]->getIntersection(&shadowRay, &tempIntersection);
    }

    if (!shadowIntersectionResult) {
      lightAngle = (light->mPosition - intersection.mPosition).unit();
      lightStrength = fmax(0.0, lightAngle * intersection.mNormal);
      lightAccumulator += (light->mColor * lightStrength);
    }
  }

  return lightAccumulator;
}

void RayTracer::saveOutput() {
  mOutput->savePng("output_001.png");
}

ImageBuffer* RayTracer::getOutput() {
  return mOutput;
}
