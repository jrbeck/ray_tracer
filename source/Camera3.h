#pragma once

#include "math/Vec3.h"
#include "math/Ray3.h"

#define CAMERA_FLOAT_T double

enum ViewportCorners {
  topLeft = 0,
  topRight = 1,
  bottomLeft = 2,
  bottomRight = 3
};

class Camera3 {
public:
  Camera3(Vec3 postion, Vec3 target, Vec3 up, CAMERA_FLOAT_T fieldOfView, CAMERA_FLOAT_T aspectRatio);

  void viewport(Vec3* vec3s) const;

private:
  Vec3 mPosition;
  Vec3 mTarget;
  Vec3 mUp;

  CAMERA_FLOAT_T mFieldOfView;
  CAMERA_FLOAT_T mAspectRatio;
};
