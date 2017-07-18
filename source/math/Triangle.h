#pragma once

#include "Shape.h"
#include "Vec3.h"
#include "Ray3.h"

class Triangle : public Shape {
public:
  Triangle();
  Triangle(const Vec3& a, const Vec3& b, const Vec3& c);

  static bool rayTriangleIntersect(const Ray3& ray, const Vec3& v0, const Vec3& v1, const Vec3& v2, VEC3_DATA_TYPE& t);
  bool getIntersection(const Ray3* ray, Intersection* intersection);
  constexpr const static VEC3_DATA_TYPE kEpsilon = 0.00001;
  Vec3 mVertices[3];
  // Vec3 mNormal;
};
