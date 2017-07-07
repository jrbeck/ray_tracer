#include "Ray3.h"

Ray3::Ray3() :
  mOrigin(Vec3(0.0, 0.0, 0.0)),
  mDirection(Vec3(1.0, 0.0, 0.0))
{}

Ray3::Ray3(Vec3 origin, Vec3 direction) :
  mOrigin(origin),
  mDirection(direction)
{}

// assumption: mDirection is a unit Vec3
void Ray3::compute(VEC3_DATA_TYPE t, Vec3* position) const {
  position->x = mOrigin.x + (t * mDirection.x);
  position->y = mOrigin.y + (t * mDirection.y);
  position->z = mOrigin.z + (t * mDirection.z);
}
