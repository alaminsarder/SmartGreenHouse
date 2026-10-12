#include "sky.h"
#include "utils.h"

// ============================================================
//  SKY GRADIENT
// ============================================================
void drawSky()
{
    Color topColor, botColor;

    if (dayTime < 0.2f)   // Night
    {
        topColor = Color(0.02f, 0.02f, 0.12f);
        botColor = Color(0.04f, 0.04f, 0.20f);
    }
    else if (dayTime < 0.35f) // Sunrise
    {
        float t = (dayTime - 0.2f) / 0.15f;
        topColor = Color(0.02f + t*0.2f, 0.02f + t*0.3f, 0.12f + t*0.5f);
        botColor = Color(t, 0.6f*t, 0.2f);
    }
    else if (dayTime < 0.65f) // Day
    {
        topColor = Color(0.12f, 0.47f, 0.86f);
        botColor = Color(0.39f, 0.71f, 1.0f);
    }
    else if (dayTime < 0.8f)  // Sunset
    {
        float t = (dayTime - 0.65f) / 0.15f;
        topColor = Color(0.12f + t*0.08f, 0.47f - t*0.45f, 0.86f - t*0.74f);
        botColor = Color(1.0f - t*0.78f, 0.59f - t*0.55f, 0.2f - t*0.08f);
    }
    else // Night
    {
        topColor = Color(0.02f, 0.02f, 0.12f);
        botColor = Color(0.04f, 0.04f, 0.20f);
    }

    drawGradientRect(0, 0, WIN_W, WIN_H, topColor, botColor);
}

// ============================================================
//  SUN
// ============================================================
void drawSun(float dayProgress)
{
    if (dayProgress < 0.2f || dayProgress > 0.8f) return;

    float angle = (dayProgress - 0.25f) * M_PI;
    float sunX  = 200 + (WIN_W - 400) * dayProgress;
    float sunY  = 450 - 350 * sin(angle);
    float brightness = sin((dayProgress - 0.2f) / 0.6f * M_PI);

    // Sun glow
    if (brightness > 0.3f)
    {
        for (int i = 5; i >= 1; i--)
        {
            float r = 30 + i * 10;
            setColor(1.0f, 0.95f, 0.4f, 0.05f * brightness);
            drawCircle(sunX, sunY, r, 40);
        }
    }

    // Sun rays
    if (brightness > 0.3f)
    {
        for (int i = 0; i < 12; i++)
        {
            float a = i * 30.0f + animTime * 15.0f;
            float rad = a * M_PI / 180.0f;
            float r1 = 38 + 5 * brightness;
            float r2 = 60 + 10 * brightness;

            setColor(1.0f, 0.94f, 0.4f, 0.4f * brightness);
            drawLine(sunX + cos(rad)*r1, sunY + sin(rad)*r1,
                     sunX + cos(rad)*r2, sunY + sin(rad)*r2, 2);
        }
    }

    // Sun body
    setColor(1.0f, 0.88f + 0.12f*brightness, 0.2f + 0.3f*brightness);
    drawCircle(sunX, sunY, 30 + 5 * brightness, 40);
}

// ============================================================
//  MOON
// ============================================================
void drawMoon(float dayProgress)
{
    if (dayProgress > 0.2f && dayProgress < 0.8f) return;

    float mx = (dayProgress < 0.2f) ? 150 : WIN_W - 150;
    float my = 100;

    // Moon glow
    setColor(0.9f, 0.9f, 0.8f, 0.08f);
    drawCircle(mx, my, 50, 40);

    // Moon body
    setColor(0.9f, 0.9f, 0.78f);
    drawCircle(mx, my, 25, 40);

    // Craters
    setColor(0.78f, 0.78f, 0.68f);
    drawCircle(mx - 8, my - 5, 4, 15);
    drawCircle(mx + 5, my + 3, 3, 15);
    drawCircle(mx - 2, my + 8, 2.5f, 15);
}

// ============================================================
//  STARS
// ============================================================
void drawStars()
{
    bool isNight = (dayTime < 0.2f || dayTime > 0.8f);
    if (!isNight) return;

    float starAlpha = 1.0f;
    if (dayTime > 0.75f && dayTime < 0.8f)
        starAlpha = (dayTime - 0.75f) / 0.05f;
    if (dayTime > 0.2f && dayTime < 0.25f)
        starAlpha = (0.25f - dayTime) / 0.05f;
    starAlpha = clamp(starAlpha, 0, 1);

    // Fixed seed for consistent star positions
    srand(42);
    for (int i = 0; i < 100; i++)
    {
        float sx = rand() % WIN_W;
        float sy = rand() % 400;
        float twinkle = 0.5f + 0.5f * sin(animTime * 2 + i * 1.7f);

        setColor(1, 1, 0.87f, starAlpha * twinkle);
        drawCircle(sx, sy, 1.2f + twinkle * 0.8f, 8);
    }
    srand((unsigned)time(nullptr));
}

// ============================================================
//  CLOUDS
// ============================================================
void drawClouds()
{
    if (dayTime < 0.25f || dayTime > 0.75f) return;

    float cloudAlpha = sin((dayTime - 0.25f) / 0.5f * M_PI) * 0.7f;

    for (int c = 0; c < 3; c++)
    {
        float cx = fmod(200.0f + c * 350 + animTime * 15, (float)(WIN_W + 200)) - 100;
        float cy = 50 + c * 45;

        setColor(1, 1, 1, cloudAlpha);
        drawCircle(cx, cy, 30, 25);
        drawCircle(cx + 30, cy - 5, 25, 25);
        drawCircle(cx + 55, cy, 28, 25);
        drawCircle(cx + 20, cy - 18, 22, 25);
        drawCircle(cx + 45, cy - 12, 20, 25);
    }
}

// ============================================================
//  HILLS / LANDSCAPE
// ============================================================
void drawHills()
{
    // Hill 1
    setColor(0.2f, 0.4f, 0.2f, 0.7f);
    drawTriangle(-50, 600, 300, 340, 700, 600);

    // Hill 2
    setColor(0.16f, 0.35f, 0.18f, 0.7f);
    drawTriangle(400, 600, 750, 370, 1050, 600);

    // Ground
    setColor(0.25f, 0.45f, 0.2f, 0.5f);
    drawRect(0, 570, WIN_W, 130);
}