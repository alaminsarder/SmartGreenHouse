#ifndef PLANT_H
#define PLANT_H

#include "globals.h"

void initPlants();
void updatePlants(float dt);
void drawPlants();
void drawSinglePlant(Plant& p);

#endif // PLANTS_H