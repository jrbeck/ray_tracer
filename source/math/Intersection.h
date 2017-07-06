#pragma once

#include "Vec3.h"

class Intersection {
public:
  Vec3 mPosition;
  Vec3 mNormal;

  Intersection();
  Intersection(Vec3 position, Vec3 normal);
};
