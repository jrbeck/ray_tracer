#include "Sphere.h"

Sphere::Sphere(Vec3 center, VEC3_DATA_TYPE radius) :
  mCenter(center),
  mRadius(radius)
{}

bool Sphere::getIntersection(const Ray3* ray, Intersection* intersection) {
  VEC3_DATA_TYPE deltaX = ray->mOrigin.x - mCenter.x;
  VEC3_DATA_TYPE deltaY = ray->mOrigin.y - mCenter.y;
  VEC3_DATA_TYPE deltaZ = ray->mOrigin.z - mCenter.z;

  VEC3_DATA_TYPE A = (ray->mDirection.x * ray->mDirection.x) + (ray->mDirection.y * ray->mDirection.y) + (ray->mDirection.z * ray->mDirection.z);
  VEC3_DATA_TYPE B = 2.0 * ((ray->mDirection.x * deltaX) + (ray->mDirection.y * deltaY) + (ray->mDirection.z * deltaZ));
  VEC3_DATA_TYPE C = (deltaX * deltaX) + (deltaY * deltaY) + (deltaZ * deltaZ) - (mRadius * mRadius);

  VEC3_DATA_TYPE discriminant = (B * B) - (4.0 * A * C);
  if (discriminant < 0.0) {
    return false;
  }

  discriminant = sqrt(discriminant);

  VEC3_DATA_TYPE t = (-B - discriminant) * (.5 * A);
  if (t > 0.0) {
    ray->compute(t, &(intersection->mPosition));
    intersection->mNormal = (intersection->mPosition - mCenter).unit();
    return true;
  }

  t = (-B + discriminant) * (.5 * A);
  if (t > 0.0) {
    ray->compute(t, &(intersection->mPosition));
    intersection->mNormal = (intersection->mPosition - mCenter).unit();
    return true;
  }

  return false;
}
