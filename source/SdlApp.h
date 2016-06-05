#pragma once

#include <iostream>

#include <SDL2/SDL.h>

#include "ImageBuffer.h"

#define SCREEN_WIDTH  (512)
#define SCREEN_HEIGHT (512)

class SdlApp {
public:
  SdlApp();
  ~SdlApp();

  int init(unsigned width, unsigned height);
  int quit();

  int drawFrame(const ImageBuffer* imageBuffer);

private:
  SDL_Window* mSdlWindow;
  SDL_Renderer* mSdlRenderer;
  SDL_Texture* mSdlTexture;

  void destroyTexture();
};
