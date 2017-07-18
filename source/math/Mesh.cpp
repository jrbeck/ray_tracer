#include "Mesh.h"

bool Mesh::getIntersection(const Ray3* ray, Intersection* intersection) {
  size_t numTriangles = mTriangles.size();
  for (size_t i = 0; i < numTriangles; i++) {
    // test for triangle intersection
    // check to see if 't' is of min value, store if it is
  }
}

bool Mesh::addVertex(Vec3& vec3, int* vertexIndex) {
  if (vertexIndex != nullptr) {
    *vertexIndex = (int)mVertices.size();
  }
  mVertices.push_back(vec3);
  return true;
}

bool Mesh::addTriangle(int* vertexIndices, int* triangleIndex) {
  if (triangleIndex != nullptr) {
    *triangleIndex = (int)(mTriangles.size() / 3);
  }
  for (int i = 0; i < 3; i++) {
    mTriangles.push_back(vertexIndices[i]);
  }
  return true;
}
