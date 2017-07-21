#pragma once

#include "Vec3.h"

class Light {
public:
  Light(Vec3 position, Vec3 color, VEC3_DATA_TYPE strength);

  Vec3 mPosition;
  Vec3 mColor;
  VEC3_DATA_TYPE mStrength;
};
