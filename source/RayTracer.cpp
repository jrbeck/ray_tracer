#include "RayTracer.h"

RayTracer::RayTracer(int width, int height, Scene* scene) :
  mOutput(nullptr),
  mWidth(width),
  mHeight(height),
  mRayThreads(nullptr),
  mScene(scene)
{
  mOutput = new ImageBuffer();
  mOutput->resize(mWidth, mHeight);


  mRayThreads = new RayThread[kNumThreads];
  for (unsigned t = 0; t < kNumThreads; t++) {
    mRayThreads[t].state = RayThreadStates::dormant;
  }

  Vec3 position(2.0, 15.0, 0.0), target(0.2, 0.0, 0.0), up(0.0, 1.0, 0.0);
  VEC3_DATA_TYPE fieldOfView = M_PI * 0.5;
  VEC3_DATA_TYPE aspectRatio = (VEC3_DATA_TYPE)mWidth / (VEC3_DATA_TYPE)mHeight;
  mCamera = new Camera3(position, target, up, fieldOfView, aspectRatio);
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
  if (mRayThreads != nullptr) {
    delete [] mRayThreads;
    mRayThreads = nullptr;
  }
}

void RayTracer::drawFrame() const {
  mAngle += 0.02;
  mCamera->mPosition.x = 15.0 * cos(mAngle);
  mCamera->mPosition.z = 15.0 * sin(mAngle);
  Vec3 viewport[4];
  mCamera->viewport(viewport);

  Vec3 leftEdge, rightEdge;

  for (unsigned j = 0; j < mHeight;) {
    for (unsigned t = 0; (t < kNumThreads) && (j < mHeight); t++) {
      if (mRayThreads[t].state == RayThreadStates::working) {
        continue;
      }

      if (mRayThreads[t].state == RayThreadStates::complete) {
        mRayThreads[t].thread.join();
      }

      mRayThreads[t].state = RayThreadStates::working;
      mRayThreads[t].j = j;
      mRayThreads[t].leftEdge = Vec3::lerp(viewport[ViewportCorners::topLeft], viewport[ViewportCorners::bottomLeft], (VEC3_DATA_TYPE)j / ((VEC3_DATA_TYPE)(mHeight - 1.0)));
      mRayThreads[t].rightEdge = Vec3::lerp(viewport[ViewportCorners::topRight], viewport[ViewportCorners::bottomRight], (VEC3_DATA_TYPE)j / ((VEC3_DATA_TYPE)(mHeight - 1.0)));
      mRayThreads[t].thread = std::thread(&RayTracer::drawScanline, this, &mRayThreads[t]);
      ++j;
    }
  }

  for (unsigned t = 0; t < kNumThreads; t++) {
    if (mRayThreads[t].state == RayThreadStates::working || mRayThreads[t].state == RayThreadStates::complete) {
      mRayThreads[t].thread.join();
      mRayThreads[t].state = RayThreadStates::dormant;
    }
  }
}

void RayTracer::drawScanline(RayThread* rayThread) const {
  for (unsigned i = 0; i < mWidth; ++i) {
    Vec3 pixelPosition = Vec3::lerp(rayThread->leftEdge, rayThread->rightEdge, (VEC3_DATA_TYPE)i / ((VEC3_DATA_TYPE)(mWidth - 1.0)));
    Ray3 ray = Ray3(mCamera->mPosition, (pixelPosition - mCamera->mPosition).unit());

    Vec3 color = traceRay(ray, 0);
    mOutput->setRgb(i, rayThread->j,
      255.0 * fmin(color.x, 1.0),
      255.0 * fmin(color.y, 1.0),
      255.0 * fmin(color.z, 1.0)
    );
  }

  rayThread->state = RayThreadStates::complete;
}

Vec3 RayTracer::traceRay(const Ray3& ray, int bounce) const {
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
    if (!tempIntersectionResult) {
      continue;
    }
    tempIntersectionDistance = Vec3::dist(ray.mOrigin, tempIntersection.mPosition);
    if (tempIntersectionDistance < intersectionDistance) {
      intersectionDistance = tempIntersectionDistance;
      intersectionResult = true;
      intersection = tempIntersection;
      intersectionShapeIndex = shapeIndex;
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
        // this poses a problem for self-shadowing meshes
        continue;
      }
      shadowIntersectionResult = shadowIntersectionResult || mScene->mShapes[shapeIndex]->getIntersection(&shadowRay, &tempIntersection);
    }

    if (!shadowIntersectionResult) {
      Vec3 intersectionToLight = light->mPosition - intersection.mPosition;
      VEC3_DATA_TYPE distanceToLight = intersectionToLight.length();
      lightAngle = intersectionToLight.unit();
      lightStrength = light->mStrength * fmax(0.0, lightAngle * intersection.mNormal) / (4.0 * M_PI * distanceToLight * distanceToLight);
      lightAccumulator += (light->mColor * lightStrength);
    }
  }

  if (bounce < kMaxBounces) {
    Vec3 bounceDirection;
    ray.mDirection.reflect(&intersection.mNormal, &bounceDirection);
    Ray3 bounceRay = Ray3(intersection.mPosition, bounceDirection);
    // this is to avoid some float errors
    bounceRay.compute(0.01, &bounceRay.mOrigin);
    lightAccumulator += traceRay(bounceRay, bounce + 1);
    // lightAccumulator = lightAccumulator * 0.5;
  }

  return lightAccumulator;
}

void RayTracer::saveOutput() {
  mOutput->savePng("output_001.png");
}

ImageBuffer* RayTracer::getOutput() {
  return mOutput;
}
