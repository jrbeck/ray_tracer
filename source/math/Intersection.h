#pragma once

#include "Vec3.h"

class Intersection {
public:
  Vec3 mPosition;
  Vec3 mNormal;
  VEC3_DATA_TYPE mTime;

  Intersection();
  Intersection(Vec3 position, Vec3 normal, VEC3_DATA_TYPE time);
};
