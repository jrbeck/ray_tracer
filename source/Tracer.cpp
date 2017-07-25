#include <iostream>
#include <vector>
#include <string>

#include <cstdio>

#include "lib/lodepng.h"

#include "Camera3.h"
#include "Pixel.h"
#include "RayTracer.h"
#include "ImageBuffer.h"
#include "Scene.h"
#include "math/PseudoRandom.h"
#include "math/Ray3.h"
#include "math/Sphere.h"
#include "math/Triangle.h"
#include "math/Mesh.h"
#include "math/Intersection.h"
#include "SdlApp.h"

void showIt(RayTracer* rayTracer) {
  SdlApp sdlApp;
  if (sdlApp.init(rayTracer->getOutput()->getWidth(), rayTracer->getOutput()->getHeight()) != 0) {
    printf("ERROR: could not initialize SdlApp\n");
    return;
  }

  Uint32 startTicks = SDL_GetTicks();
  Uint32 frameCount = 0;

  SDL_Event e;
  bool quit = false;
  while (!quit) {
    rayTracer->drawFrame();
    sdlApp.drawFrame(rayTracer->getOutput());
    frameCount++;

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }
      if (e.type == SDL_KEYDOWN) {
        switch(e.key.keysym.sym) {
          case SDLK_ESCAPE:
            quit = true;
            break;
          case SDLK_q:
            break;
          case SDLK_a:
            break;
          default:
            break;
        }
      }
      if (e.type == SDL_MOUSEBUTTONDOWN) {
        Uint32 endTicks = SDL_GetTicks();
        printf("average FPS: %.3f\n", ((double)(1000 * frameCount) / (double)(endTicks - startTicks)));
      }
    }
  }

  Uint32 endTicks = SDL_GetTicks();
  printf("average FPS: %.3f\n", ((double)(1000 * frameCount) / (double)(endTicks - startTicks)));

  sdlApp.quit();
}


void testViewport() {
  int screenW = 512;
  int screenH = 512;

  Vec3 position(0.0f, 0.0f, 0.0f), target(1.0f, 0.1f, 0.0f), up(0.0f, 1.0f, 0.0f);
  VEC3_DATA_TYPE fieldOfView = M_PI * 0.5;
  VEC3_DATA_TYPE aspectRatio = 1.0;

  Camera3* camera = new Camera3(position, target, up, fieldOfView, aspectRatio);

  target.print("Target");

  Vec3 viewport[4];
  camera->viewport(viewport);
  for (int i = 0; i < 4; i++) {
    viewport[i].print();
  }

  printf("--------\n");

  Vec3 leftEdge, rightEdge;

  for (int j = 0; j < screenH; j++) {
    leftEdge = Vec3::lerp(viewport[ViewportCorners::topLeft], viewport[ViewportCorners::bottomLeft], (double)j / ((double)(screenH - 1.0)));
    rightEdge = Vec3::lerp(viewport[ViewportCorners::topRight], viewport[ViewportCorners::bottomRight], (double)j / ((double)(screenH - 1.0)));
    for (int i = 0; i <  screenW; i++) {
      Vec3 temp = Vec3::lerp(leftEdge, rightEdge, (double)i / ((double)(screenW - 1.0)));
      printf("(%.2f,%.2f,%.2f)", temp.x, temp.y, temp.z);
    }
    printf("\n");
  }

  delete camera;
}


void testIntersection() {
  Ray3 ray = Ray3(Vec3(0.0, 0.0, 0.0), Vec3(-1.0, 0.0, 0.0));
  Sphere sphere = Sphere(Vec3(20.0, 0.5, 0.0), 1.0);
  Intersection intersection;
  bool result = sphere.getIntersection(&ray, &intersection);
  printf("%d\n", result);
  if (result) {
    intersection.mPosition.print("pos");
    intersection.mNormal.print("norm");
  }
}

void testReflection() {
  Vec3 a = Vec3(1, 0, 0);
  Vec3 normal = Vec3(-0.7071, 0.7071, 0);
  Vec3 result; // a - (normal * ((a * normal) * 2));

  a.reflect(&normal, &result);
  result.print("r");
}

void testTriangleIntersection() {
  Triangle tri;
  tri.mVertices[0] = Vec3(1.0, 1.0, 1.0);
  tri.mVertices[1] = Vec3(-1.0, 1.0, 1.0);
  tri.mVertices[2] = Vec3(-1.0, -1.0, 1.0);

  Ray3 ray = Ray3(Vec3(-0.5, 0.5, 0.0), Vec3(0.0, 0.0, 1.0));

  Intersection intersection;
  bool result = tri.getIntersection(&ray, &intersection);
  printf("%d\n", result);
  if (result) {
    intersection.mPosition.print("pos");
    intersection.mNormal.print("norm");
  }
}

void testMeshIntersection() {
  Mesh mesh;
  Vec3 v[4];
  int indices[4];
  v[0] = Vec3(0, 1, 0);
  v[1] = Vec3(-1, 0, 1);
  v[2] = Vec3(1, 0, 1);
  v[3] = Vec3(0.1, 0, -1);
  for (int i = 0; i < 4; i++) {
    mesh.addVertex(v[i], &indices[i]);
  }
  mesh.addTriangle(indices[0], indices[1], indices[2], nullptr);
  mesh.addTriangle(indices[0], indices[2], indices[3], nullptr);
  mesh.addTriangle(indices[0], indices[3], indices[1], nullptr);
  mesh.addTriangle(indices[1], indices[2], indices[3], nullptr);

  mesh.print();

  // Ray3 ray = Ray3(Vec3(0.0, 0.5, -2.0), Vec3(0.0, 0.0, 1.0));
  Ray3 ray = Ray3(Vec3(0.0, 0.5, 2.0), Vec3(0.0, 0.0, -1.0));

  Intersection intersection;
  bool result = mesh.getIntersection(&ray, &intersection);
  printf("%d\n", result);
  if (result) {
    intersection.mPosition.print("pos");
    intersection.mNormal.print("norm");
  }
}


Scene* buildTestScene() {
  Scene* scene = new Scene();
  scene->addLight(new Light(Vec3(0.0, 150.0, 0.0), Vec3(1.0, 0.0, 0.0), 1000.0));
  scene->addLight(new Light(Vec3(0.0, 0.0, 5.0), Vec3(0.0, 1.0, 1.0), 250.0));
  scene->addLight(new Light(Vec3(5.0, 5.0, 0.0), Vec3(1.0, 0.0, 1.0), 250.0));
  scene->addLight(new Light(Vec3(5.0, 0.0, 5.0), Vec3(0.0, 1.0, 0.0), 250.0));
  scene->addLight(new Light(Vec3(10.0, 5.0, 5.0), Vec3(0.0, 0.0, 1.0), 250.0));

  PseudoRandom pseudoRandom = PseudoRandom(3);
  for (int i = 0; i < 5; ++i) {
    VEC3_DATA_TYPE x = pseudoRandom.nextDouble(-5.0, 5.0);
    VEC3_DATA_TYPE y = pseudoRandom.nextDouble(-5.0, 5.0);
    VEC3_DATA_TYPE z = pseudoRandom.nextDouble(-5.0, 5.0);
    VEC3_DATA_TYPE r = pseudoRandom.nextDouble(0.5, 2.0);
    scene->addShape(new Sphere(Vec3(x, y, z), r));
  }

  // TETRAHEDRON
  Mesh* mesh = new Mesh;
  Vec3 v[4];
  int indices[4];
  v[0] = Vec3(0, 1, 0);
  v[1] = Vec3(-1, 0, 1);
  v[2] = Vec3(1, 0, 1);
  v[3] = Vec3(0.1, 0, -1);
  for (int i = 0; i < 4; i++) {
    mesh->addVertex(v[i], &indices[i]);
  }
  mesh->addTriangle(indices[0], indices[1], indices[2], nullptr);
  mesh->addTriangle(indices[0], indices[2], indices[3], nullptr);
  mesh->addTriangle(indices[0], indices[3], indices[1], nullptr);
  mesh->addTriangle(indices[1], indices[3], indices[2], nullptr);
  scene->addShape(mesh);


  // PLANE
  // Mesh* mesh = new Mesh;
  mesh = new Mesh;
  // Vec3 v[4];
  // int indices[4];
  VEC3_DATA_TYPE side = 15.0;
  VEC3_DATA_TYPE y = -5.0;
  v[0] = Vec3(-side, y, side);
  v[1] = Vec3(side, y, side);
  v[2] = Vec3(-side, y, -side);
  v[3] = Vec3(side, y, -side);
  for (int i = 0; i < 4; i++) {
    mesh->addVertex(v[i], &indices[i]);
  }
  mesh->addTriangle(indices[0], indices[1], indices[3], nullptr);
  mesh->addTriangle(indices[3], indices[2], indices[0], nullptr);
  scene->addShape(mesh);

  return scene;
}


int main(int nargs, char** argv) {
  // testViewport();
  // testIntersection();
  // testReflection();
  // testTriangleIntersection();
  // testMeshIntersection();

  // *********************************************************

  int screenW = 1024;
  int screenH = 1024;

  Scene* scene = buildTestScene();
  RayTracer* rayTracer = new RayTracer(screenW, screenH, scene);
  showIt(rayTracer);
  rayTracer->saveOutput();
  delete rayTracer;
  delete scene;

  // *********************************************************

  return 0;
}
