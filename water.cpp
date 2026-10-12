#include "water.h"
#include "utils.h"
#include <cmath>
#include <algorithm>

float waterTankLevel = 100.0f;

static void drawEllipseFilled(float cx, float cy, float rx, float ry, int segs,
                               Color c)
{
    glBegin(GL_POLYGON);
    glColor4f(c.r, c.g, c.b, c.a);
    for (int i = 0; i < segs; i++)
    {
        float a = (float)i / segs * 2.0f * (float)M_PI;
        glVertex2f(cx + cosf(a) * rx, cy + sinf(a) * ry);
    }
    glEnd();
}

static void drawEllipseOutline(float cx, float cy, float rx, float ry, int segs,
                                Color c, float lw = 1.5f)
{
    setColor(c);
    glLineWidth(lw);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segs; i++)
    {
        float a = (float)i / segs * 2.0f * (float)M_PI;
        glVertex2f(cx + cosf(a) * rx, cy + sinf(a) * ry);
    }
    glEnd();
}

static void drawPipeH(float x, float y, float w, float thick, Color base)
{
    setColor(base.r * 0.55f, base.g * 0.55f, base.b * 0.55f, base.a);
    drawRect(x, y - thick / 2 - 1, w, thick + 2);

    setColor(base);
    drawRect(x, y - thick / 2, w, thick);

    setColor(std::min(base.r*1.35f,1.0f), std::min(base.g*1.35f,1.0f), std::min(base.b*1.35f,1.0f), base.a);
    drawRect(x, y - thick / 2, w, thick * 0.3f);

    setColor(base.r * 0.4f, base.g * 0.4f, base.b * 0.4f, base.a);
    drawRect(x, y + thick / 2 - thick * 0.2f, w, thick * 0.2f);
}

static void drawPipeV(float x, float y, float h, float thick, Color base)
{
    setColor(base.r * 0.55f, base.g * 0.55f, base.b * 0.55f, base.a);
    drawRect(x - thick / 2 - 1, y, thick + 2, h);

    setColor(base);
    drawRect(x - thick / 2, y, thick, h);

    setColor(std::min(base.r*1.35f,1.0f), std::min(base.g*1.35f,1.0f), std::min(base.b*1.35f,1.0f), base.a);
    drawRect(x - thick / 2, y, thick * 0.3f, h);

    setColor(base.r * 0.4f, base.g * 0.4f, base.b * 0.4f, base.a);
    drawRect(x + thick / 2 - thick * 0.2f, y, thick * 0.2f, h);
}

static void drawFlangeJoint(float x, float y, float r, Color base)
{
    setColor(base.r * 0.4f, base.g * 0.4f, base.b * 0.4f, base.a);
    drawCircle(x, y, r + 1.5f, 16);
    setColor(std::min(base.r*1.2f,1.0f), std::min(base.g*1.2f,1.0f), std::min(base.b*1.2f,1.0f), base.a);
    drawCircle(x, y, r, 14);

    setColor(0.15f, 0.15f, 0.18f, base.a);
    for (int i = 0; i < 4; i++)
    {
        float a = i * 90.0f * (float)M_PI / 180.0f + 0.5f;
        drawCircle(x + cosf(a) * r * 0.65f, y + sinf(a) * r * 0.65f, 0.9f, 6);
    }
}

static const Color PIPE_COLOR = Color(0.42f, 0.47f, 0.56f, 1.0f);

void updateWaterSystem(float dt)
{
    if (sensor.sprinklerOn && waterTankLevel > 0)
    {
        waterTankLevel -= dt * 2.0f;
        waterTankLevel = clamp(waterTankLevel, 0, 100);
    }

    if (isRaining && waterTankLevel < 100)
    {
        waterTankLevel += dt * 5.0f;
        waterTankLevel = clamp(waterTankLevel, 0, 100);
    }

    if (waterTankLevel < 30 && !isRaining)
    {
        waterTankLevel += dt * 3.0f;
        waterTankLevel = clamp(waterTankLevel, 0, 100);
    }
}

void drawWaterTank()
{
    float tx = 795;
    float ty = 300;      // moved higher (was 345)
    float tw = 46;
    float th = 128;
    float rx = tw / 2.0f;
    float capRy = 7.0f;

    float cx = tx + rx;
    float topY = ty;
    float botY = ty + th;

    setColor(0, 0, 0, 0.25f);
    drawEllipseFilled(cx + 3, botY + capRy + 10, rx + 6, 5, 20, Color(0, 0, 0, 0.25f));

    setColor(0.22f, 0.25f, 0.28f);
    drawRect(tx - 2, botY + capRy - 2, 5, 26);
    drawRect(tx + tw - 3, botY + capRy - 2, 5, 26);
    setColor(0.35f, 0.38f, 0.42f);
    drawLine(tx - 1, botY + capRy + 6, tx + tw, botY + capRy + 14, 1.5f);
    drawLine(tx + tw - 1, botY + capRy + 6, tx - 1, botY + capRy + 14, 1.5f);

    setColor(0.28f, 0.30f, 0.33f);
    drawRect(tx - 6, botY + capRy + 22, tw + 12, 5);

    Color shellLight(0.88f, 0.90f, 0.93f, 1.0f);
    Color shellDark(0.60f, 0.64f, 0.70f, 1.0f);

    drawGradientRect(tx, topY, tw, th, shellLight, shellDark);

    setColor(1.0f, 1.0f, 1.0f, 0.5f);
    drawRect(tx + tw * 0.12f, topY, tw * 0.10f, th);
    setColor(0.45f, 0.48f, 0.53f, 0.55f);
    drawRect(tx + tw * 0.80f, topY, tw * 0.12f, th);

    setColor(0.50f, 0.53f, 0.58f, 0.6f);
    drawLine(tx, topY + th * 0.33f, tx + tw, topY + th * 0.33f, 1);
    drawLine(tx, topY + th * 0.66f, tx + tw, topY + th * 0.66f, 1);

    drawEllipseFilled(cx, topY, rx, capRy, 24, Color(0.72f, 0.76f, 0.82f, 1.0f));
    drawEllipseOutline(cx, topY, rx, capRy, 24, Color(0.40f, 0.44f, 0.50f, 1.0f), 1.4f);

    drawEllipseFilled(cx, botY, rx, capRy, 24, Color(0.45f, 0.49f, 0.55f, 1.0f));
    drawEllipseOutline(cx, botY, rx, capRy, 24, Color(0.30f, 0.33f, 0.38f, 1.0f), 1.4f);

    drawEllipseFilled(cx, topY - 1.5f, rx * 0.55f, capRy * 0.55f, 18,
                       Color(0.35f, 0.40f, 0.48f, 1.0f));
    drawEllipseOutline(cx, topY - 1.5f, rx * 0.55f, capRy * 0.55f, 18,
                        Color(0.55f, 0.60f, 0.68f, 1.0f), 1.2f);
    setColor(0.25f, 0.28f, 0.33f);
    drawRect(cx - 3, topY - 6, 6, 2.5f);
    drawCircle(cx - 3, topY - 6, 1.2f, 6);
    drawCircle(cx + 3, topY - 6, 1.2f, 6);

    float innerRx = rx - 4;
    float waterHeight = (th - 2 * capRy) * (waterTankLevel / 100.0f);
    float waterTopY = botY - waterHeight;
    if (waterTopY < topY + capRy) waterTopY = topY + capRy;

    if (waterTankLevel > 1.0f)
    {
        Color waterTop, waterBottom;
        if (waterTankLevel > 60)      { waterTop = Color(0.35f,0.72f,0.98f,0.9f); waterBottom = Color(0.08f,0.38f,0.78f,0.95f); }
        else if (waterTankLevel > 30) { waterTop = Color(0.40f,0.75f,0.92f,0.9f); waterBottom = Color(0.15f,0.45f,0.75f,0.95f); }
        else                           { waterTop = Color(0.55f,0.60f,0.78f,0.9f); waterBottom = Color(0.40f,0.48f,0.68f,0.95f); }

        drawGradientRect(tx + 3, waterTopY, tw - 6, botY - capRy * 0.5f - waterTopY,
                          waterTop, waterBottom);

        drawEllipseFilled(cx, botY, innerRx, capRy * 0.85f, 20, waterBottom);

        int segs = 24;
        glBegin(GL_POLYGON);
        glColor4f(waterTop.r, waterTop.g, waterTop.b, waterTop.a);
        for (int i = 0; i < segs; i++)
        {
            float a = (float)i / segs * 2.0f * (float)M_PI;
            float ripple = sinf(animTime * 2.2f + a * 3.0f) * 0.8f;
            glVertex2f(cx + cosf(a) * innerRx, waterTopY + sinf(a) * capRy * 0.8f + ripple);
        }
        glEnd();

        setColor(0.75f, 0.92f, 1.0f, 0.6f);
        glLineWidth(1.2f);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < segs; i++)
        {
            float a = (float)i / segs * 2.0f * (float)M_PI;
            float ripple = sinf(animTime * 2.2f + a * 3.0f) * 0.8f;
            glVertex2f(cx + cosf(a) * innerRx, waterTopY + sinf(a) * capRy * 0.8f + ripple);
        }
        glEnd();

        setColor(1.0f, 1.0f, 1.0f, 0.35f);
        drawEllipseFilled(cx - innerRx * 0.3f, waterTopY - 1.0f, innerRx * 0.25f, 1.5f, 10,
                           Color(1,1,1,0.35f));

        for (int i = 0; i < 4; i++)
        {
            float bt = fmod(animTime * 0.7f + i * 0.33f, 1.0f);
            float bx = tx + 10 + (i % 3) * 10;
            float by = botY - bt * waterHeight;
            if (by > waterTopY && by < botY - capRy * 0.5f)
            {
                setColor(0.85f, 0.95f, 1.0f, 0.5f * (1 - bt));
                drawCircle(bx, by, 1.2f, 8);
            }
        }

        setColor(1.0f, 1.0f, 1.0f, 0.18f);
        drawRect(tx + 6, waterTopY + 2, 3, botY - capRy - waterTopY - 4);
    }

    setColor(0.35f, 0.38f, 0.43f, 0.8f);
    drawLine(tx, topY, tx, botY, 1.3f);
    drawLine(tx + tw, topY, tx + tw, botY, 1.3f);

    setColor(0.55f, 0.58f, 0.63f, 0.9f);
    for (int m = 1; m < 4; m++)
    {
        float my = topY + capRy + (th - 2 * capRy) * (1.0f - m * 0.25f);
        drawLine(tx + tw + 2, my, tx + tw + 6, my, 1.2f);
    }

    setColor(0.40f, 0.44f, 0.50f);
    drawRect(tx + tw - 2, topY + 6, 10, 4);
    if (waterTankLevel >= 99.5f && isRaining)
    {
        float t = fmod(animTime * 2.0f, 1.0f);
        setColor(0.5f, 0.8f, 1.0f, 0.8f * (1 - t));
        drawCircle(tx + tw + 8, topY + 8 + t * 14, 1.4f, 8);
    }

    float gutterX = 773;
    float inletY = topY + 14;
    drawPipeH(gutterX, inletY, tx - gutterX, 4.5f, Color(0.38f,0.55f,0.65f,1.0f));
    drawFlangeJoint(gutterX, inletY, 3.5f, Color(0.38f,0.55f,0.65f,1.0f));
    drawFlangeJoint(tx, inletY, 3.5f, Color(0.38f,0.55f,0.65f,1.0f));

    if (isRaining)
    {
        for (int i = 0; i < 3; i++)
        {
            float t = fmod(animTime * 2.6f + i * 0.33f, 1.0f);
            float fx = gutterX + t * (tx - gutterX);
            setColor(0.55f, 0.80f, 1.0f, 0.85f);
            drawCircle(fx, inletY, 1.3f, 8);
        }
    }

    float gx = tx - 14, gy = topY + 28;
    setColor(0.18f, 0.18f, 0.20f);
    drawCircle(gx, gy, 8, 16);
    setColor(0.85f, 0.85f, 0.88f);
    drawCircle(gx, gy, 6.5f, 16);
    setColor(0.3f, 0.3f, 0.35f);
    drawCircleOutline(gx, gy, 6.5f, 16);

    float needleAngle = 200.0f + (waterTankLevel / 100.0f) * 140.0f;
    float nra = needleAngle * (float)M_PI / 180.0f;
    Color needleColor = waterTankLevel < 30 ? Color(0.9f,0.2f,0.2f,1) : Color(0.15f,0.6f,0.2f,1);
    setColor(needleColor);
    drawLine(gx, gy, gx + cosf(nra) * 5, gy + sinf(nra) * 5, 1.6f);
    setColor(0.1f, 0.1f, 0.1f);
    drawCircle(gx, gy, 1, 8);

    setColor(0.15f, 0.22f, 0.30f, 0.9f);
    drawRect(tx - 2, topY + 44, tw + 4, 20);
    setColor(0.75f, 0.92f, 1.0f);
    drawText("WATER", tx + 4, topY + 57);
    setColor(0.45f, 0.72f, 0.90f);
    drawText("TANK", tx + 7, topY + 70);

    if (waterTankLevel < 30)
    {
        float blink = 0.5f + 0.5f * sin(animTime * 5);
        setColor(1.0f, 0.2f, 0.2f, blink);
        drawCircle(cx, topY - 9, 2.6f, 10);
        setColor(1.0f, 0.3f, 0.3f, blink * 0.3f);
        drawCircle(cx, topY - 9, 5, 12);
    }
    else
    {
        setColor(0.2f, 1.0f, 0.3f, 0.9f);
        drawCircle(cx, topY - 9, 1.8f, 8);
    }

    setColor(0, 0, 0, 0.6f);
    drawRect(tx - 6, botY + 30, tw + 12, 16);
    Color pctColor = waterTankLevel < 30 ? Color(1.0f,0.4f,0.4f)
                    : waterTankLevel < 60 ? Color(1.0f,0.9f,0.3f)
                    : Color(0.4f,1.0f,0.5f);
    setColor(pctColor);
    std::string levelStr = "H2O: " + floatToStr(waterTankLevel, 0) + "%";
    drawText(levelStr, tx - 2, botY + 42);

    // Pump box - clamped to a fixed safe Y so it never collides with
    // the sprinkler riser pipes even when the tank moves up/down.
    float pumpX = tx + 3, pumpW = tw - 6, pumpH = 20;
    float pumpY = std::max(botY + capRy + 30, 513.0f);

    drawPipeV(cx, botY + capRy, pumpY - (botY + capRy), 5, PIPE_COLOR);

    setColor(0.22f, 0.24f, 0.27f);
    drawRect(pumpX, pumpY, pumpW, pumpH);
    setColor(0.35f, 0.38f, 0.42f);
    drawRectOutline(pumpX, pumpY, pumpW, pumpH, 1.3f);

    setColor(0.15f, 0.15f, 0.18f);
    drawCircle(pumpX + pumpW * 0.3f, pumpY + pumpH * 0.5f, 5, 14);
    setColor(0.45f, 0.48f, 0.52f);
    drawCircleOutline(pumpX + pumpW * 0.3f, pumpY + pumpH * 0.5f, 5, 14);

    bool pumpRunning = sensor.sprinklerOn && waterTankLevel > 0;
    float motorSpin = pumpRunning ? animTime * 700.0f : 0.0f;
    setColor(0.6f, 0.62f, 0.65f);
    for (int i = 0; i < 3; i++)
    {
        float a = (motorSpin + i * 120.0f) * (float)M_PI / 180.0f;
        drawLine(pumpX + pumpW * 0.3f, pumpY + pumpH * 0.5f,
                 pumpX + pumpW * 0.3f + cosf(a) * 4, pumpY + pumpH * 0.5f + sinf(a) * 4, 1.3f);
    }

    setColor(pumpRunning ? Color(0.2f,1.0f,0.3f,0.9f) : Color(0.5f,0.1f,0.1f,0.8f));
    drawCircle(pumpX + pumpW * 0.75f, pumpY + pumpH * 0.5f, 2.2f, 10);

    if (pumpRunning)
    {
        float vib = sinf(animTime * 40.0f) * 0.6f;
        setColor(0.22f, 0.24f, 0.27f, 0.5f);
        drawRect(pumpX + vib, pumpY - 2, pumpW, 2);
    }

    drawPipeV(cx, pumpY + pumpH, 8, 5, PIPE_COLOR);
}

void drawWaterPipes()
{
    float mainY = 508;
    float mainX1 = 60, mainX2 = 834;
    float riserTopY = 465;

    drawPipeV(829, 478, 30, 7, PIPE_COLOR);
    drawFlangeJoint(829, 478, 5, PIPE_COLOR);

    float valveX = 829, valveY = 492;
    setColor(0.55f, 0.28f, 0.14f);
    drawCircle(valveX, valveY, 6.5f, 16);
    setColor(0.78f, 0.48f, 0.20f);
    drawCircleOutline(valveX, valveY, 6.5f, 16);

    float handleAngle = sensor.sprinklerOn ? 0.0f : 90.0f;
    float ha = handleAngle * (float)M_PI / 180.0f;
    setColor(0.30f, 0.20f, 0.10f);
    drawLine(valveX - cosf(ha) * 7, valveY - sinf(ha) * 7,
             valveX + cosf(ha) * 7, valveY + sinf(ha) * 7, 2.6f);
    setColor(0.15f, 0.10f, 0.05f);
    drawCircle(valveX, valveY, 1.6f, 8);

    drawPipeH(mainX1, mainY, mainX2 - mainX1, 7, PIPE_COLOR);

    for (int i = 0; i <= 6; i++)
    {
        float jx = mainX1 + i * ((mainX2 - mainX1) / 6.0f);
        drawFlangeJoint(jx, mainY, 4.5f, PIPE_COLOR);
    }

    if (sensor.sprinklerOn && waterTankLevel > 0)
    {
        for (int i = 0; i < 12; i++)
        {
            float t = fmod(animTime * 2.2f + i * 0.09f, 1.0f);
            float flowX = mainX2 - t * (mainX2 - mainX1);
            setColor(0.45f, 0.75f, 1.0f, 0.8f);
            drawCircle(flowX, mainY, 1.8f, 8);
            setColor(0.75f, 0.92f, 1.0f, 0.5f);
            drawCircle(flowX - 2, mainY - 0.5f, 0.8f, 6);
        }

        for (int j = 0; j < 4; j++)
        {
            float t = fmod(animTime * 3.0f + j * 0.25f, 1.0f);
            float flowY = 480 + t * 28;
            setColor(0.45f, 0.75f, 1.0f, 0.8f);
            drawCircle(829, flowY, 1.8f, 8);
        }
    }

    for (int i = 0; i < 6; i++)
    {
        float px = 158.0f + i * 130;
        float cxp = px + 3.5f;

        drawPipeV(cxp, riserTopY, mainY - riserTopY, 6.5f, PIPE_COLOR);

        drawFlangeJoint(cxp, riserTopY, 4.5f, PIPE_COLOR);

        setColor(0.30f, 0.34f, 0.40f);
        drawRect(px - 3, mainY, 13, 8);
        setColor(0.52f, 0.56f, 0.62f);
        drawRectOutline(px - 3, mainY, 13, 8, 1);
        setColor(0.15f, 0.15f, 0.18f);
        drawCircle(px - 1, mainY + 4, 0.8f, 6);
        drawCircle(px + 9, mainY + 4, 0.8f, 6);

        if (sensor.sprinklerOn && waterTankLevel > 0)
        {
            for (int j = 0; j < 3; j++)
            {
                float t = fmod(animTime * 3.2f + j * 0.33f + i * 0.12f, 1.0f);
                float dropY = mainY - t * (mainY - riserTopY);

                setColor(0.4f, 0.75f, 1.0f, 0.85f);
                drawCircle(cxp, dropY, 1.8f, 8);
                setColor(0.8f, 0.93f, 1.0f, 0.5f);
                drawCircle(cxp - 1, dropY - 1, 0.7f, 6);
            }

            float dt2 = fmod(animTime * 1.8f + i * 0.4f, 1.0f);
            setColor(0.5f, 0.8f, 1.0f, 0.6f * (1 - dt2));
            drawCircle(cxp, mainY + 10 + dt2 * 6, 1.3f, 8);
        }
    }
}

void drawWaterLevel()
{
    if (waterTankLevel < 30)
    {
        float blink = 0.5f + 0.5f * sin(animTime * 4);

        setColor(0.8f, 0.2f, 0.1f, 0.85f * blink);
        drawRect(WIN_W / 2 - 150, 45, 300, 30);
        setColor(1.0f, 0.5f, 0.5f);
        drawRectOutline(WIN_W / 2 - 150, 45, 300, 30, 2);

        setColor(1.0f, 1.0f, 0.3f, blink);
        drawTriangle(WIN_W / 2 - 130, 68, WIN_W / 2 - 120, 50, WIN_W / 2 - 110, 68);
        setColor(0, 0, 0);
        drawText("!", WIN_W / 2 - 122, 65);

        setColor(1, 1, 1);
        drawText("LOW WATER! Refilling tank...", WIN_W / 2 - 95, 65);
    }
}