#include <iostream>
#include <vector>
#include <string>

#include <cstdio>

#include "lib/lodepng.h"

#include "Pixel.h"
#include "RayTracer.h"
#include "ImageBuffer.h"
#include "math/PseudoRandom.h"
#include "SdlApp.h"

void showIt(RayTracer* rayTracer) {
  SdlApp sdlApp;
  if (sdlApp.init(rayTracer->getOutput()->getWidth(), rayTracer->getOutput()->getHeight()) != 0) {
    printf("ERROR: could not initialize SdlApp\n");
    return;
  }

  SDL_Event e;
  bool quit = false;
  while (!quit) {
    sdlApp.drawFrame(rayTracer->getOutput());

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
            // ditherDistance++;
            // printf("dither: %d\n", ditherDistance);
            break;
          case SDLK_a:
            // if (ditherDistance > 0) {
            //   ditherDistance--;
            // }
            // printf("dither: %d\n", ditherDistance);
            break;
          default:
            break;
        }

      }
      if (e.type == SDL_MOUSEBUTTONDOWN) {
      }
    }
  }

  sdlApp.quit();
}

int main(int argc, char** argv) {
  RayTracer* rayTracer = new RayTracer(512, 512);

  showIt(rayTracer);

  // rayTracer->printResults();
  rayTracer->saveOutput();

  delete rayTracer;
}
