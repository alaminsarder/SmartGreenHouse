#include "globals.h"
#include "window.h"
#include "utils.h"
#include "sky.h"
#include "greenhouse.h"
#include "plant.h"
#include "sensors.h"
#include "rain.h"
#include "fan.h"
#include "sprinkler.h"
#include "led.h"
#include "dashboard.h"
#include "water.h"
#include "person.h"      // ← NEW!

bool fanManualOverride = false;
bool fanManualState = false;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawSky();
    drawStars();
    drawSun(dayTime);
    drawMoon(dayTime);
    drawClouds();
    drawHills();

    drawGreenhouse();
    drawWaterPipes();
    drawLEDs();
    drawPlants();
    drawPeople();          // ← NEW! Walking people (day only)
    drawSprinklers();
    drawFan();
    drawWindEffect();
    drawWaterTank();

    drawRain();
    drawRainSplash();

    drawDashboard();
    drawTopBar();
    drawBottomBar();
    drawWaterLevel();
}

void update(float dt)
{
    dayTime = fmod(dayTime + daySpeed, 1.0f);

    updateSensors(dt);
    updateAutomation();

    if (fanManualOverride)
        sensor.fanOn = fanManualState;

    updatePlants(dt);
    updatePeople(dt);       // ← NEW!
    updateRain(dt);
    updateWaterSystem(dt);
}

void onKeyDown(int key)
{
    switch (key)
    {
    case 'R':
        isRaining = !isRaining;
        rainTimer = 0;
        std::cout << "[INPUT] Rain: " << (isRaining ? "ON" : "OFF") << "\n";
        break;

    case 'D':
        daySpeed *= 2.0f;
        if (daySpeed > 0.01f) daySpeed = 0.01f;
        break;

    case 'N':
        daySpeed *= 0.5f;
        if (daySpeed < 0.00005f) daySpeed = 0.00005f;
        break;

    case 'W':
        waterTankLevel = 100.0f;
        std::cout << "[INPUT] Water tank refilled!\n";
        break;

    case 'E':
        waterTankLevel = 20.0f;
        std::cout << "[INPUT] Water level low!\n";
        break;

    case 'F':
        fanManualOverride = true;
        fanManualState = !sensor.fanOn;
        sensor.fanOn = fanManualState;
        std::cout << "[INPUT] Fan MANUAL: " << (fanManualState ? "ON" : "OFF") << "\n";
        break;

    case 'A':
        fanManualOverride = false;
        std::cout << "[INPUT] Fan back to AUTO mode\n";
        break;
    }
}

void onSpecialKey(int key)
{
    switch (key)
    {
    case VK_UP:
        sensor.temperature += 2;
        sensor.temperature = clamp(sensor.temperature, 8, 45);
        std::cout << "[INPUT] Temp: " << sensor.temperature << "\n";
        break;

    case VK_DOWN:
        sensor.temperature -= 2;
        sensor.temperature = clamp(sensor.temperature, 8, 45);
        break;

    case VK_RIGHT:
        sensor.soilMoisture += 8;
        sensor.soilMoisture = clamp(sensor.soilMoisture, 0, 100);
        std::cout << "[INPUT] Moisture: " << sensor.soilMoisture << "\n";
        break;

    case VK_LEFT:
        sensor.soilMoisture -= 8;
        sensor.soilMoisture = clamp(sensor.soilMoisture, 0, 100);
        break;
    }
}

int main()
{
    std::cout << "========================================\n";
    std::cout << "  SMART GREENHOUSE AUTOMATION SYSTEM\n";
    std::cout << "========================================\n";
    std::cout << "  CONTROLS:\n";
    std::cout << "    UP/DOWN Arrow    - Temperature\n";
    std::cout << "    LEFT/RIGHT Arrow - Soil Moisture\n";
    std::cout << "    R                - Rain toggle\n";
    std::cout << "    D                - Speed up Day\n";
    std::cout << "    N                - Slow down Day\n";
    std::cout << "    W                - Refill Water Tank\n";
    std::cout << "    E                - Empty Water Tank\n";
    std::cout << "    F                - Fan Manual Toggle\n";
    std::cout << "    A                - Fan Auto Mode\n";
    std::cout << "    ESC              - Exit\n";
    std::cout << "========================================\n\n";

    srand((unsigned)time(nullptr));

    if (!createGLWindow("Smart Greenhouse Automation System", WIN_W, WIN_H)) {
        std::cerr << "Window create korte parlam na!\n";
        return 1;
    }

    initPlants();
    initRain();
    initPeople();          // ← NEW!

    runMainLoop();

    destroyGLWindow();

    return 0;
}