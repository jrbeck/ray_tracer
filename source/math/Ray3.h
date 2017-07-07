#pragma once

#include "Vec3.h"

class Ray3 {
public:
  Vec3 mOrigin;
  Vec3 mDirection;

  Ray3();
  Ray3(Vec3 origin, Vec3 direction);

  void compute(VEC3_DATA_TYPE t, Vec3* position) const;
};
