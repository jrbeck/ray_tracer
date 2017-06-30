#include "Camera3.h"

Camera3::Camera3(Vec3 position, Vec3 target, Vec3 up, CAMERA_FLOAT_T fieldOfView, CAMERA_FLOAT_T aspectRatio) :
  mPosition(position),
  mTarget(target),
  mUp(up.unit()),
  mFieldOfView(fieldOfView),
  mAspectRatio(aspectRatio)
{}

void Camera3::viewport(Vec3* vec3s) const {
  Vec3 look = mTarget - mPosition;
  Vec3 left = Vec3::cross(look, mUp);
  Vec3 up = Vec3::cross(left, mTarget);
  CAMERA_FLOAT_T halfScreenWidth = look.mag() * sin(mFieldOfView * 0.5);
  CAMERA_FLOAT_T halfScreenHeight = halfScreenWidth / mAspectRatio;

  Vec3 leftVec = left.unit() * halfScreenWidth;
  Vec3 upVec = up * halfScreenHeight;

  vec3s[ViewportCorners::topLeft] = mTarget + leftVec + upVec;
  vec3s[ViewportCorners::topRight] = mTarget - leftVec + upVec;
  vec3s[ViewportCorners::bottomLeft] = mTarget + leftVec - upVec;
  vec3s[ViewportCorners::bottomRight] = mTarget - leftVec - upVec;
}
