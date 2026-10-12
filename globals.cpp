#include "globals.h"

float animTime  = 0.0f;
float dayTime   = 0.35f;
float daySpeed  = 0.00015f;

bool  isRaining  = false;
float rainTimer  = 0.0f;
float nextRainAt = 15.0f;

SensorData sensor = {
    24.0f, 65.0f, 75.0f,
    false, false, false
};

std::vector<Plant>    plants;
std::vector<RainDrop> rainDrops;

float deltaTime = 0.0f;

GLuint fontBase = 0;