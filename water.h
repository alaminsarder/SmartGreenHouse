#ifndef WATER_H
#define WATER_H

#include "globals.h"

// Water system components
void drawWaterTank();
void drawWaterPipes();
void drawWaterLevel();
void updateWaterSystem(float dt);

// Water tank state
extern float waterTankLevel;  // 0-100%

#endif // WATER_H