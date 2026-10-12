#include "plant.h"
#include "utils.h"

void initPlants()
{
    plants.clear();

    for (int i = 0; i < 8; i++)
    {
        Plant p;
        p.x         = 90.0f + i * 90;
        p.y         = 510.0f;
        p.growth    = 0.05f + (rand() % 10) / 100.0f;
        p.maxHeight = 90.0f + (rand() % 30);
        p.stemColor = Color(0.24f, 0.51f, 0.20f);
        p.leafColor = Color(0.16f + (rand()%15)/100.f,
                             0.55f + (rand()%25)/100.f,
                             0.16f + (rand()%15)/100.f);

        int colorType = rand() % 5;
        switch(colorType) {
            case 0: p.flowerColor = Color(1.0f, 0.4f, 0.6f);   break;
            case 1: p.flowerColor = Color(1.0f, 0.85f, 0.2f);  break;
            case 2: p.flowerColor = Color(0.8f, 0.3f, 0.9f);   break;
            case 3: p.flowerColor = Color(1.0f, 0.3f, 0.3f);   break;
            case 4: p.flowerColor = Color(1.0f, 0.6f, 0.2f);   break;
        }

        plants.push_back(p);
    }

    for (int i = 0; i < 6; i++)
    {
        Plant p;
        p.x         = 130.0f + i * 100;
        p.y         = 490.0f;
        p.growth    = 0.03f + (rand() % 8) / 100.0f;
        p.maxHeight = 70.0f + (rand() % 20);
        p.stemColor = Color(0.20f, 0.45f, 0.18f);
        p.leafColor = Color(0.14f + (rand()%12)/100.f,
                             0.50f + (rand()%22)/100.f,
                             0.14f + (rand()%12)/100.f);

        int colorType = rand() % 5;
        switch(colorType) {
            case 0: p.flowerColor = Color(1.0f, 0.4f, 0.6f);   break;
            case 1: p.flowerColor = Color(1.0f, 0.85f, 0.2f);  break;
            case 2: p.flowerColor = Color(0.8f, 0.3f, 0.9f);   break;
            case 3: p.flowerColor = Color(1.0f, 0.3f, 0.3f);   break;
            case 4: p.flowerColor = Color(0.5f, 0.8f, 1.0f);   break;
        }

        plants.push_back(p);
    }
}

void updatePlants(float dt)
{
    bool goodConditions =
        (sensor.soilMoisture > 25 && sensor.soilMoisture < 90) &&
        (sensor.temperature > 12 && sensor.temperature < 38);

    for (auto& p : plants)
    {
        if (goodConditions && p.growth < 1.0f)
        {
            float rate = 0.00012f;

            if (sensor.lightLevel > 40 || sensor.ledOn)
                rate *= 1.6f;

            if (sensor.soilMoisture > 50 && sensor.soilMoisture < 75)
                rate *= 1.4f;

            if (sensor.temperature > 20 && sensor.temperature < 30)
                rate *= 1.25f;

            if (isRaining)
                rate *= 1.3f;

            p.growth = clamp(p.growth + rate * dt * 60, 0, 1);
        }

        if (p.growth >= 1.0f)
        {
            if (rand() % 4000 == 0)
            {
                p.growth = 0.05f;
            }
        }
    }
}

void drawSinglePlant(Plant& p)
{
    float g = p.growth;
    float h = p.maxHeight * g;

    if (h < 3)
    {
        setColor(0.6f, 0.4f, 0.2f);
        drawCircle(p.x, p.y - 2, 3, 10);
        return;
    }

    setColor(p.stemColor);
    drawRect(p.x - 2.5f, p.y - h, 5, h);

    setColor(p.stemColor.r * 1.3f, p.stemColor.g * 1.3f, p.stemColor.b * 1.3f);
    drawRect(p.x - 2.5f, p.y - h, 1.5f, h);

    setColor(p.stemColor.r * 0.7f, p.stemColor.g * 0.7f, p.stemColor.b * 0.7f);
    drawRect(p.x + 1, p.y - h, 1.5f, h);

    if (g > 0.20f)
    {
        float leafSize = 25.0f * g;

        setColor(p.leafColor);
        glBegin(GL_POLYGON);
            glVertex2f(p.x, p.y - h * 0.35f);
            glVertex2f(p.x - leafSize, p.y - h * 0.35f - leafSize * 0.6f);
            glVertex2f(p.x - leafSize * 0.3f, p.y - h * 0.35f - leafSize * 1.1f);
            glVertex2f(p.x, p.y - h * 0.35f - h * 0.25f);
        glEnd();

        glBegin(GL_POLYGON);
            glVertex2f(p.x, p.y - h * 0.55f);
            glVertex2f(p.x + leafSize, p.y - h * 0.55f - leafSize * 0.6f);
            glVertex2f(p.x + leafSize * 0.3f, p.y - h * 0.55f - leafSize * 1.1f);
            glVertex2f(p.x, p.y - h * 0.55f - h * 0.2f);
        glEnd();

        setColor(p.leafColor.r * 0.6f, p.leafColor.g * 0.6f, p.leafColor.b * 0.6f);
        drawLine(p.x, p.y - h * 0.35f,
                 p.x - leafSize * 0.6f, p.y - h * 0.35f - leafSize * 0.7f, 1);
        drawLine(p.x, p.y - h * 0.55f,
                 p.x + leafSize * 0.6f, p.y - h * 0.55f - leafSize * 0.7f, 1);
    }

    if (g > 0.40f)
    {
        float ls2 = 18.0f * g;
        setColor(p.leafColor.r * 0.85f, p.leafColor.g * 1.1f, p.leafColor.b * 0.85f);

        glBegin(GL_TRIANGLES);
            glVertex2f(p.x, p.y - h * 0.20f);
            glVertex2f(p.x - ls2, p.y - h * 0.20f - ls2 * 0.5f);
            glVertex2f(p.x, p.y - h * 0.20f - ls2 * 0.8f);
        glEnd();

        glBegin(GL_TRIANGLES);
            glVertex2f(p.x, p.y - h * 0.70f);
            glVertex2f(p.x + ls2, p.y - h * 0.70f - ls2 * 0.5f);
            glVertex2f(p.x, p.y - h * 0.70f - ls2 * 0.8f);
        glEnd();
    }

    if (g > 0.55f)
    {
        float canopyR = 18.0f * g;

        setColor(p.leafColor.r, clamp(p.leafColor.g + 0.15f, 0, 1),
                 p.leafColor.b, 0.9f);
        drawCircle(p.x, p.y - h - 3, canopyR, 20);

        setColor(p.leafColor.r * 0.9f, p.leafColor.g * 1.05f, p.leafColor.b * 0.9f);
        drawCircle(p.x - canopyR * 0.5f, p.y - h + 2, canopyR * 0.6f, 15);

        drawCircle(p.x + canopyR * 0.5f, p.y - h + 2, canopyR * 0.6f, 15);

        setColor(p.leafColor.r * 0.6f, p.leafColor.g * 0.6f,
                 p.leafColor.b * 0.6f, 0.25f);
        drawCircle(p.x + 3, p.y - h - 1, canopyR * 0.6f, 15);
    }

    if (g > 0.70f && g < 0.85f)
    {
        float budSize = 3.0f + g * 3.0f;

        setColor(p.flowerColor.r * 0.7f, p.flowerColor.g * 0.7f,
                 p.flowerColor.b * 0.7f, 0.9f);
        drawCircle(p.x - 12, p.y - h - 8, budSize, 12);
        drawCircle(p.x + 12, p.y - h - 6, budSize, 12);
        drawCircle(p.x, p.y - h - 12, budSize + 1, 12);
    }

    if (g > 0.80f)
    {
        float alpha = clamp((g - 0.80f) / 0.15f, 0, 1);
        float mainFy = p.y - h - 15 * g;

        for (int i = 0; i < 8; i++)
        {
            float ang = i * 45.0f * M_PI / 180.0f + animTime * 0.2f;
            float pr  = 10 * alpha;
            float px  = p.x + cos(ang) * pr;
            float py  = mainFy + sin(ang) * pr;

            setColor(p.flowerColor.r, p.flowerColor.g, p.flowerColor.b, 0.9f * alpha);
            drawCircle(px, py, 6 * alpha, 15);

            setColor(p.flowerColor.r + 0.2f, p.flowerColor.g + 0.2f,
                     p.flowerColor.b + 0.2f, 0.5f * alpha);
            drawCircle(px - 1, py - 1, 3 * alpha, 12);
        }

        setColor(1.0f, 0.85f, 0.1f, 0.95f * alpha);
        drawCircle(p.x, mainFy, 5 * alpha, 15);

        setColor(1.0f, 1.0f, 0.6f, 0.7f * alpha);
        drawCircle(p.x - 1, mainFy - 1, 2 * alpha, 10);

        setColor(0.8f, 0.4f, 0.1f, 0.9f * alpha);
        for (int i = 0; i < 6; i++)
        {
            float ang = i * 60.0f * M_PI / 180.0f;
            drawCircle(p.x + cos(ang) * 2, mainFy + sin(ang) * 2, 0.8f, 6);
        }

        if (g > 0.88f)
        {
            float sideAlpha = clamp((g - 0.88f) / 0.12f, 0, 1);

            float lfx = p.x - 18;
            float lfy = p.y - h - 5;

            for (int i = 0; i < 6; i++)
            {
                float ang = i * 60.0f * M_PI / 180.0f;
                float pr = 5 * sideAlpha;
                float px = lfx + cos(ang) * pr;
                float py = lfy + sin(ang) * pr;

                setColor(p.flowerColor.r, p.flowerColor.g, p.flowerColor.b, 0.85f * sideAlpha);
                drawCircle(px, py, 3.5f * sideAlpha, 12);
            }
            setColor(1.0f, 0.85f, 0.2f, 0.9f * sideAlpha);
            drawCircle(lfx, lfy, 2.5f * sideAlpha, 12);

            float rfx = p.x + 18;
            float rfy = p.y - h - 3;

            for (int i = 0; i < 6; i++)
            {
                float ang = i * 60.0f * M_PI / 180.0f;
                float pr = 5 * sideAlpha;
                float px = rfx + cos(ang) * pr;
                float py = rfy + sin(ang) * pr;

                setColor(p.flowerColor.r, p.flowerColor.g, p.flowerColor.b, 0.85f * sideAlpha);
                drawCircle(px, py, 3.5f * sideAlpha, 12);
            }
            setColor(1.0f, 0.85f, 0.2f, 0.9f * sideAlpha);
            drawCircle(rfx, rfy, 2.5f * sideAlpha, 12);
        }

        if (g > 0.95f)
        {
            for (int i = 0; i < 4; i++)
            {
                float ang = (i * 90.0f + animTime * 40) * M_PI / 180.0f;
                float dist = 20 + sin(animTime * 3 + i) * 3;
                float sx = p.x + cos(ang) * dist;
                float sy = mainFy + sin(ang) * dist;

                float sparkle = 0.5f + 0.5f * sin(animTime * 4 + i * 1.5f);
                setColor(1.0f, 1.0f, 0.8f, 0.7f * sparkle);
                drawCircle(sx, sy, 1.5f, 8);
            }
        }
    }
}

void drawPlants()
{
    for (int i = 8; i < (int)plants.size(); i++)
        drawSinglePlant(plants[i]);

    for (int i = 0; i < 8 && i < (int)plants.size(); i++)
        drawSinglePlant(plants[i]);
}