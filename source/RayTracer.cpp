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
  mScene->addLight(new Light(Vec3(5.0, 5.0, 0.0), Vec3(1.0, 0.0, 1.0)));
  mScene->addLight(new Light(Vec3(5.0, 0.0, 5.0), Vec3(0.0, 1.0, 0.0)));
  mScene->addLight(new Light(Vec3(10.0, 5.0, 5.0), Vec3(0.0, 0.0, 1.0)));

  PseudoRandom pseudoRandom = PseudoRandom(6);
  for (int i = 0; i < 20; ++i) {
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

  size_t numThreads = 20;
  RayThread mainRayThread;
  RayThread* rayThreads = new RayThread[numThreads];

  for (unsigned j = 0; j < mHeight;) {
    for (unsigned t = 0; (t < numThreads) && (j < mHeight); t++) {
      rayThreads[t].active = true;
      rayThreads[t].j = j;
      rayThreads[t].leftEdge = Vec3::lerp(viewport[ViewportCorners::topLeft], viewport[ViewportCorners::bottomLeft], (VEC3_DATA_TYPE)j / ((VEC3_DATA_TYPE)(mHeight - 1.0)));
      rayThreads[t].rightEdge = Vec3::lerp(viewport[ViewportCorners::topRight], viewport[ViewportCorners::bottomRight], (VEC3_DATA_TYPE)j / ((VEC3_DATA_TYPE)(mHeight - 1.0)));
      rayThreads[t].thread = std::thread(&RayTracer::drawScanline, this, &rayThreads[t]);
      ++j;
    }

    // if (j < mHeight) {
    //   mainRayThread.j = j;
    //   mainRayThread.leftEdge = Vec3::lerp(viewport[ViewportCorners::topLeft], viewport[ViewportCorners::bottomLeft], (VEC3_DATA_TYPE)j / ((VEC3_DATA_TYPE)(mHeight - 1.0)));
    //   mainRayThread.rightEdge = Vec3::lerp(viewport[ViewportCorners::topRight], viewport[ViewportCorners::bottomRight], (VEC3_DATA_TYPE)j / ((VEC3_DATA_TYPE)(mHeight - 1.0)));
    //   drawScanline(&mainRayThread);
    //   ++j;
    // }

    for (unsigned t = 0; t < numThreads; t++) {
      if (rayThreads[t].active) {
        rayThreads[t].thread.join();
        rayThreads[t].active = false;
      }
    }
  }
}

void RayTracer::drawScanline(RayThread* rayThread) const {
  for (unsigned i = 0; i < mWidth; ++i) {
    Vec3 pixelPosition = Vec3::lerp(rayThread->leftEdge, rayThread->rightEdge, (VEC3_DATA_TYPE)i / ((VEC3_DATA_TYPE)(mWidth - 1.0)));
    Ray3 ray = Ray3(mCamera->mPosition, (pixelPosition - mCamera->mPosition).unit());

    Vec3 color = traceRay(ray);
    mOutput->setRgb(i, rayThread->j,
      255.0 * fmin(color.x, 1.0),
      255.0 * fmin(color.y, 1.0),
      255.0 * fmin(color.z, 1.0)
    );
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
      Vec3 intersectionToLight = light->mPosition - intersection.mPosition;
      lightAngle = intersectionToLight.unit();
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
