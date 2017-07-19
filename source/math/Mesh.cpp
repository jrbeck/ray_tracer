#include "Mesh.h"

bool Mesh::getIntersection(const Ray3* ray, Intersection* intersection) {
  VEC3_DATA_TYPE t_min = 1000000.0, t;
  Vec3 normal;
  size_t numTriangles = mTriangles.size() / 3;
  for (size_t i = 0; i < numTriangles; i++) {
    size_t triangleIndex = i * 3;
    if (Triangle::rayTriangleIntersect(*ray, mVertices[mTriangles[triangleIndex]], mVertices[mTriangles[triangleIndex + 1]], mVertices[mTriangles[triangleIndex + 2]], t)) {
      if (t > 0.0 && t < t_min) {
        t_min = t;

        Vec3 v0v1 = mVertices[mTriangles[triangleIndex + 1]] - mVertices[mTriangles[triangleIndex]];
        Vec3 v0v2 = mVertices[mTriangles[triangleIndex + 2]] - mVertices[mTriangles[triangleIndex]];
        normal = (v0v1.cross(v0v2)).unit();
      }
    }
  }

  if (t_min > 0.0 && t_min < 1000000.0) {
    ray->compute(t_min, &(intersection->mPosition));
    intersection->mNormal = normal;
    return true;
  }

  return false;
}

bool Mesh::addVertex(Vec3& vec3, int* vertexIndex) {
  if (vertexIndex != nullptr) {
    *vertexIndex = (int)mVertices.size();
  }
  mVertices.push_back(vec3);
  return true;
}

bool Mesh::addTriangle(int a, int b, int c, int* triangleIndex) {
  if (triangleIndex != nullptr) {
    *triangleIndex = (int)(mTriangles.size() / 3);
  }
  mTriangles.push_back(a);
  mTriangles.push_back(b);
  mTriangles.push_back(c);
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

void Mesh::print() {
  char scratch[64];
  printf("Vertices ----------\n");
  for (size_t i = 0; i < mVertices.size(); i++) {
    sprintf(scratch, "%d", i);
    mVertices[i].print(scratch);
  }
  printf("\nTriangles ----------\n");
  size_t numTriangles = mTriangles.size() / 3;
  for (size_t i = 0; i < numTriangles; i++) {
    size_t triangleIndex = i * 3;
    printf("%d: %d, %d, %d\n", i, mTriangles[triangleIndex], mTriangles[triangleIndex + 1], mTriangles[triangleIndex + 2]);
  }
  printf("----------\n");
}
