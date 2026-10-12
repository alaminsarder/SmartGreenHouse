#include "sensors.h"
#include "utils.h"
#include "water.h"     // ← NEW include

static float sensorTimer = 0.0f;

void updateSensors(float dt)
{
    bool isDay = (dayTime > 0.2f && dayTime < 0.8f);

    // Light Level
    if (isDay)
    {
        float sunBrightness = sin((dayTime - 0.2f) / 0.6f * M_PI);
        sensor.lightLevel = sunBrightness * 90.0f + 10.0f;
        if (isRaining)
            sensor.lightLevel *= 0.5f;
    }
    else
    {
        sensor.lightLevel = 3.0f + (rand() % 7);
    }

    sensorTimer += dt;
    if (sensorTimer > 0.5f)
    {
        sensorTimer = 0;

        float targetTemp;
        if (isDay)
        {
            float daySin = sin((dayTime - 0.3f) * M_PI);
            targetTemp = 22.0f + 16.0f * daySin;
        }
        else
        {
            targetTemp = 16.0f;
        }

        if (isRaining) targetTemp -= 4.0f;

        sensor.temperature += (targetTemp - sensor.temperature) * 0.08f;
        sensor.temperature += ((rand() % 20) - 10) * 0.1f;
        sensor.temperature = clamp(sensor.temperature, 8, 45);

        // Soil moisture
        if (isRaining)
            sensor.soilMoisture += 1.2f;
        else if (sensor.sprinklerOn && waterTankLevel > 0)  // ← Check water!
            sensor.soilMoisture += 0.8f;
        else
        {
            float evapRate = 0.2f + (sensor.temperature - 15) * 0.02f;
            sensor.soilMoisture -= evapRate;
        }

        sensor.soilMoisture = clamp(sensor.soilMoisture, 0, 100);
    }
}

void updateAutomation()
{
    sensor.fanOn = (sensor.temperature > 30.0f);
    
    // Sprinkler only works if water available!
    sensor.sprinklerOn = (sensor.soilMoisture < 35.0f && !isRaining && waterTankLevel > 5);
    
    sensor.ledOn = (sensor.lightLevel < 30.0f);
}