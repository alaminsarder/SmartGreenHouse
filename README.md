# Smart Greenhouse Automation System

## About
A visual simulation of a Smart Greenhouse that automatically 
manages plant care. The system monitors temperature, soil 
moisture, and light levels, automatically activating fan, 
sprinkler, or LED lights as needed.

## Features
- **Automatic Watering** - Sprinkler ON when soil moisture < 35%
- **Automatic Cooling** - Fan ON when temperature > 30°C
- **Automatic Lighting** - LEDs ON when light < 30% (night/cloudy)
- **Day/Night Cycle** - Realistic sun, moon, stars animation
- **Rain Animation** - Random rain with splash effects
- **Plant Growth** - Seeds → Sprout → Sapling → Mature → Bloom
- **Live Dashboard** - Real-time sensor data display

## Controls

| Key            | Action                     |
|----------------|----------------------------|
| ↑ Up Arrow     | Increase Temperature       |
| ↓ Down Arrow   | Decrease Temperature       |
| → Right Arrow  | Increase Soil Moisture     |
| ← Left Arrow   | Decrease Soil Moisture     |
| R              | Toggle Rain ON/OFF         |
| D              | Speed up Day/Night cycle   |
| N              | Slow down Day/Night cycle  |
| +/-            | Increase/Decrease Temp     |
| ESC            | Exit                       |

## Build & Run

g++ -std=c++17 -O2 -o greenhouse.exe main.cpp globals.cpp utils.cpp window.cpp sky.cpp greenhouse.cpp plant.cpp sensors.cpp rain.cpp fan.cpp sprinkler.cpp led.cpp dashboard.cpp water.cpp -lopengl32 -lglu32 -lgdi32 -luser32

.\greenhouse.exe
