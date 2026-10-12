#ifndef WINDOW_H
#define WINDOW_H

#include "globals.h"

// Window creation and OpenGL setup
bool createGLWindow(const char* title, int width, int height);
void destroyGLWindow();

// Main message loop
void runMainLoop();

// Font setup for text rendering
void buildFont();
void killFont();

// Window handle access
HWND getWindowHandle();

#endif // WINDOW_H