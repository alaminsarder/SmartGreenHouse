#include "rain.h"
#include "utils.h"

// ============================================================
//  INIT RAIN
// ============================================================
void initRain()
{
    rainDrops.clear();
    for (int i = 0; i < 200; i++)
    {
        RainDrop rd;
        rd.x      = rand() % (WIN_W + 100) - 50;
        rd.y      = rand() % WIN_H;
        rd.speed  = 6 + rand() % 8;
        rd.length = 12 + rand() % 18;
        rainDrops.push_back(rd);
    }
}

// ============================================================
//  UPDATE RAIN TIMER & STATE
// ============================================================
void updateRain(float dt)
{
    rainTimer += dt;

    if (!isRaining && rainTimer > nextRainAt)
    {
        isRaining = true;
        rainTimer = 0;
        nextRainAt = 25.0f + rand() % 30;
    }
    else if (isRaining && rainTimer > 10.0f + rand() % 10)
    {
        isRaining = false;
        rainTimer = 0;
        nextRainAt = 15.0f + rand() % 25;
    }

    // Move drops
    if (isRaining)
    {
        for (auto& rd : rainDrops)
        {
            rd.y += rd.speed;
            rd.x -= 1.5f; // Wind effect

            if (rd.y > WIN_H)
            {
                rd.y = -rd.length - rand() % 50;
                rd.x = rand() % (WIN_W + 100) - 50;
            }
            if (rd.x < -20)
                rd.x = WIN_W + rand() % 50;
        }
    }
}

// ============================================================
//  DRAW RAIN
// ============================================================
void drawRain()
{
    if (!isRaining) return;

    glLineWidth(1.5f);
    for (auto& rd : rainDrops)
    {
        setColor(0.59f, 0.78f, 1.0f, 0.6f);
        glBegin(GL_LINES);
            glVertex2f(rd.x, rd.y);
            glColor4f(0.59f, 0.78f, 1.0f, 0.1f);
            glVertex2f(rd.x + 2, rd.y + rd.length);
        glEnd();
    }
}

// ============================================================
//  RAIN SPLASH on greenhouse roof
// ============================================================
void drawRainSplash()
{
    if (!isRaining) return;

    for (int i = 0; i < 12; i++)
    {
        float t = fmod(animTime * 3 + i * 0.35f, 1.0f);
        float sx = 100 + i * 55;
        float sy = 200 + i * 5;

        setColor(0.59f, 0.78f, 1.0f, 0.5f * (1.0f - t));
        drawCircleOutline(sx, sy, 5 * t, 15);
    }

    // Ground splashes
    for (int i = 0; i < 8; i++)
    {
        float t = fmod(animTime * 4 + i * 0.5f, 1.0f);
        float gx = 60 + i * 95;
        float gy = 540;

        setColor(0.59f, 0.78f, 1.0f, 0.3f * (1.0f - t));
        drawCircleOutline(gx, gy, 4 * t, 12);
    }
}