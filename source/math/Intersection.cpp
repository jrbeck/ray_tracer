#include "Intersection.h"

Intersection::Intersection() {}

Intersection::Intersection(Vec3 position, Vec3 normal, VEC3_DATA_TYPE time) :
  mPosition(position),
  mNormal(normal),
  mTime(time)
{}
