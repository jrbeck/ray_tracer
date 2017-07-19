#pragma once

#include <vector>

#include "Shape.h"
#include "Vec3.h"
#include "Triangle.h"

class Mesh : public Shape {
public:
  bool getIntersection(const Ray3* ray, Intersection* intersection);

  bool addVertex(Vec3& vec3, int* vertexIndex);
  bool addTriangle(int a, int b, int c, int* triangleIndex);
  bool addTriangle(int* vertexIndices, int* triangleIndex);

  void print();

private:
  std::vector<Vec3> mVertices;
  std::vector<int> mTriangles;
};
