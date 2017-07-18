#include "Camera3.h"

Camera3::Camera3(Vec3 position, Vec3 target, Vec3 up, VEC3_DATA_TYPE fieldOfView, VEC3_DATA_TYPE aspectRatio) :
  mPosition(position),
  mTarget(target),
  mUp(up.unit()),
  mFieldOfView(fieldOfView),
  mAspectRatio(aspectRatio)
{}

void Camera3::viewport(Vec3* vec3s) const {
  Vec3 look = mTarget - mPosition;
  Vec3 left = Vec3::cross(look, mUp);
  Vec3 up = Vec3::cross(left, look);

  VEC3_DATA_TYPE halfScreenWidth = look.length() * sin(mFieldOfView * 0.5);
  VEC3_DATA_TYPE halfScreenHeight = halfScreenWidth / mAspectRatio;

  Vec3 leftVec = left.unit() * halfScreenWidth;
  Vec3 upVec = up.unit() * halfScreenHeight;

  vec3s[ViewportCorners::topLeft] = mTarget + leftVec + upVec;
  vec3s[ViewportCorners::topRight] = mTarget - leftVec + upVec;
  vec3s[ViewportCorners::bottomLeft] = mTarget + leftVec - upVec;
  vec3s[ViewportCorners::bottomRight] = mTarget - leftVec - upVec;
}
