#include "Ray3.h"

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
