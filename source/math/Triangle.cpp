#include "Triangle.h"

Triangle::Triangle()
{}

Triangle::Triangle(const Vec3& a, const Vec3& b, const Vec3& c)
{
  mVertices[0] = a;
  mVertices[1] = b;
  mVertices[2] = c;

  // Vec3 v0v1 = v1 - v0;
  // Vec3 v0v2 = v2 - v0;
  // mNormal =
}

// stolen from:
// http://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution
bool Triangle::rayTriangleIntersect(
    const Ray3& ray,
    const Vec3& v0, const Vec3& v1, const Vec3& v2,
    VEC3_DATA_TYPE& t)
{
  // compute plane's normal
  Vec3 v0v1 = v1 - v0;
  Vec3 v0v2 = v2 - v0;
  // no need to normalize
  Vec3 N = v0v1.cross(v0v2); // N
  // VEC3_DATA_TYPE area2 = N.length();

  // Step 1: finding P

  // check if ray and plane are parallel ?
  VEC3_DATA_TYPE NdotRayDirection = N.dot(ray.mDirection);
  if (fabs(NdotRayDirection) < kEpsilon) {
    return false; // they are parallel so they don't intersect !
  }

  // compute d parameter using equation 2
  VEC3_DATA_TYPE d = N.dot(v0);

  // compute t (equation 3)
  t = (-N.dot(ray.mOrigin) + d) / NdotRayDirection;
  // check if the triangle is behind the ray
  if (t < 0) return false; // the triangle is behind

  // compute the intersection point using equation 1
  Vec3 P = ray.mOrigin + (ray.mDirection * t);

  // Step 2: inside-outside test
  Vec3 C; // vector perpendicular to triangle's plane

  // edge 0
  Vec3 edge0 = v1 - v0;
  Vec3 vp0 = P - v0;
  C = edge0.cross(vp0);
  if (N.dot(C) < 0) return false; // P is on the right side

  // edge 1
  Vec3 edge1 = v2 - v1;
  Vec3 vp1 = P - v1;
  C = edge1.cross(vp1);
  if (N.dot(C) < 0)  return false; // P is on the right side

  // edge 2
  Vec3 edge2 = v0 - v2;
  Vec3 vp2 = P - v2;
  C = edge2.cross(vp2);
  if (N.dot(C) < 0) return false; // P is on the right side;

  return true; // this ray hits the triangle
}

bool Triangle::getIntersection(const Ray3* ray, Intersection* intersection) {
  VEC3_DATA_TYPE t;
  bool result = rayTriangleIntersect(*ray, mVertices[0], mVertices[1], mVertices[2], t);
  // intersection->
  return result;
}
