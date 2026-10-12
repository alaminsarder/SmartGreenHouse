#include "sprinkler.h"
#include "utils.h"

// ============================================================
//  DRAW SINGLE SPRINKLER
// ============================================================
void drawSingleSprinkler(int idx)
{
    // 6 sprinklers spread across the greenhouse
    float sx = 100.0f + idx * 130;
    float sy = 510.0f;

    // ---- Pipe ----
    setColor(0.50f, 0.50f, 0.55f);
    drawRect(sx + 58, sy - 45, 7, 45);

    setColor(0.55f, 0.55f, 0.60f);
    drawCircle(sx + 62, sy - 45, 4, 12);

    // ---- Head ----
    setColor(0.40f, 0.40f, 0.48f);
    drawCircle(sx + 62, sy - 52, 8, 15);
    setColor(0.50f, 0.50f, 0.58f);
    drawCircleOutline(sx + 62, sy - 52, 8, 15);

    // Nozzle dots
    setColor(0.30f, 0.30f, 0.38f);
    for (int i = 0; i < 3; i++)
    {
        float a = (i - 1) * 0.8f - 1.57f;
        drawCircle(sx + 62 + cos(a)*4, sy - 52 + sin(a)*4, 1.2f, 8);
    }

    // ---- Water spray ----
    if (sensor.sprinklerOn)
    {
        int numDrops = 15;
        for (int i = 0; i < numDrops; i++)
        {
            float t = fmod(animTime * 2.5f + i * 0.2f + idx * 0.15f, 1.0f);
            float angle = -90.0f + (i - numDrops / 2) * 15.0f;
            float rad   = angle * M_PI / 180.0f;
            float dist  = t * 55;

            float dropX = sx + 62 + cos(rad) * dist;
            float dropY = sy - 52 + sin(rad) * dist + t * t * 45;
            float alpha = (1.0f - t) * 0.85f;
            float size  = 3.0f - t * 2.0f;

            if (dropY < sy && size > 0.5f)
            {
                setColor(0.39f, 0.71f, 1.0f, alpha);
                drawCircle(dropX, dropY, size, 10);

                setColor(0.39f, 0.71f, 1.0f, alpha * 0.3f);
                drawCircle(dropX + 1, dropY - 3, size * 0.6f, 8);
            }
        }

        // Mist effect
        for (int i = 0; i < 5; i++)
        {
            float t = fmod(animTime * 3 + i * 0.6f + idx * 0.2f, 1.0f);
            setColor(0.7f, 0.85f, 1.0f, 0.12f * (1.0f - t));
            drawCircle(sx + 62 + (i - 2) * 7, sy - 52 - t * 15, 5 * t, 12);
        }
    }
}

// ============================================================
//  DRAW ALL SPRINKLERS - Now 6 sprinklers!
// ============================================================
void drawSprinklers()
{
    for (int i = 0; i < 6; i++)
        drawSingleSprinkler(i);
}