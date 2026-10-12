#ifndef UTILS_H
#define UTILS_H

#include "globals.h"

// Drawing helpers
void setColor(float r, float g, float b, float a = 1.0f);
void setColor(Color c);

void drawRect(float x, float y, float w, float h);
void drawRectOutline(float x, float y, float w, float h, float lineWidth = 2.0f);
void drawFilledRect(float x, float y, float w, float h, Color fill, Color outline, float lineW = 2.0f);

void drawCircle(float cx, float cy, float radius, int segments = 30);
void drawCircleOutline(float cx, float cy, float radius, int segments = 30);

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
void drawLine(float x1, float y1, float x2, float y2, float lineWidth = 1.0f);
void drawGradientRect(float x, float y, float w, float h, Color topColor, Color bottomColor);

// Text rendering (using Windows font)
void drawText(const std::string& text, float x, float y);
void drawTextLarge(const std::string& text, float x, float y);
void drawTextSmall(const std::string& text, float x, float y);

// Progress bar
void drawProgressBar(float x, float y, float w, float h,
                      float percent, Color fill, Color bg, Color border,
                      const std::string& label);

// Math
float lerp(float a, float b, float t);
float clamp(float val, float minV, float maxV);
std::string floatToStr(float val, int precision = 1);

#endif