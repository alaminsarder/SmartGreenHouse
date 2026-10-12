#ifndef GLOBALS_H
#define GLOBALS_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <vector>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

// ============================================================
//  WINDOW CONSTANTS
// ============================================================
const int WIN_W = 1200;
const int WIN_H = 700;

// ============================================================
//  STRUCTS
// ============================================================

struct Color {
    float r, g, b, a;
    Color(float r_=1, float g_=1, float b_=1, float a_=1)
        : r(r_), g(g_), b(b_), a(a_) {}
};

struct RainDrop {
    float x, y;
    float speed;
    float length;
};

struct Plant {
    float x, y;
    float growth;
    float maxHeight;
    Color stemColor;
    Color leafColor;
    Color flowerColor;
};

struct SensorData {
    float temperature;
    float soilMoisture;
    float lightLevel;
    bool fanOn;
    bool sprinklerOn;
    bool ledOn;
};

// ============================================================
//  EXTERN GLOBALS
// ============================================================
extern float animTime;
extern float dayTime;
extern float daySpeed;

extern bool  isRaining;
extern float rainTimer;
extern float nextRainAt;
extern bool fanManualOverride;
extern bool fanManualState;

extern SensorData sensor;
extern std::vector<Plant>    plants;
extern std::vector<RainDrop> rainDrops;

extern float deltaTime;
// Add this line at the end (before #endif)
extern float waterTankLevel;

// Font display list (Windows font rendering ke liye)
extern GLuint fontBase;

#endif // GLOBALS_H