#include <iostream>
#include <vector>
#include <string>

#include <cstdio>

#include "lib/lodepng.h"

#include "Camera3.h"
#include "Pixel.h"
#include "RayTracer.h"
#include "ImageBuffer.h"
#include "math/PseudoRandom.h"
#include "math/Ray3.h"
#include "math/Sphere.h"
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

int main(int argc, char** argv) {
  int screenW = 1024;
  int screenH = 1024;

  // testViewport();
  // testIntersection();

  // *********************************************************

  RayTracer* rayTracer = new RayTracer(screenW, screenH);
  showIt(rayTracer);
  rayTracer->saveOutput();
  delete rayTracer;

  // *********************************************************

  return 0;
}
