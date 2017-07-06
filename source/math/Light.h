#pragma once

#include "Vec3.h"

class Light {
public:
  Light(Vec3 position, Vec3 color);

  Vec3 mPosition;
  Vec3 mColor;
};
