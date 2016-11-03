/// screen managment and input handling.

#pragma once

#include <SDL.h>
#include <SDL_opengl.h>

#include "inexor/rpc/SharedTree.hpp"

#define SCR_MINW 320
#define SCR_MINH 200
#define SCR_MAXW 10000
#define SCR_MAXH 10000
#define SCR_DEFAULTW 1024
#define SCR_DEFAULTH 768

namespace inexor { namespace rendering { namespace screen {
extern SDL_Window *sdl_window;

extern int screenw, screenh;

extern SharedVar<int> fullscreen, vsync, vsynctear, colorbits, depthbits, stencilbits, fsaa;

extern void setupscreen(int &useddepthbits, int &usedfsaa);
extern void restorevsync();
extern void restoregamma();
extern void cleargamma();
extern void cleanupSDL();

}}} // ns inexor::rendering::screen

