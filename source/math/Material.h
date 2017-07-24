#pragma once

#include "Vec3.h"

class Material {
public:
  Material(Vec3 diffuseColor, VEC3_DATA_TYPE reflectivity);

  Vec3 mDiffuseColor;
  VEC3_DATA_TYPE mReflectivity;
};
