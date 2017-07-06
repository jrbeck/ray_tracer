#pragma once

#include "math/Vec3.h"
#include "math/Ray3.h"

enum ViewportCorners {
  topLeft = 0,
  topRight = 1,
  bottomLeft = 2,
  bottomRight = 3
};

class Camera3 {
public:
  Vec3 mPosition;
  Vec3 mTarget;
  Vec3 mUp;

  VEC3_DATA_TYPE mFieldOfView;
  VEC3_DATA_TYPE mAspectRatio;

  Camera3(Vec3 postion, Vec3 target, Vec3 up, VEC3_DATA_TYPE fieldOfView, VEC3_DATA_TYPE aspectRatio);

  void viewport(Vec3* vec3s) const;
};
