#include "fan.h"
#include "utils.h"

static void drawSingleFan(float fx, float fy)
{
    float pulse = sin(animTime * 4.0f) * 0.5f + 0.5f;

    if (sensor.fanOn)
    {
        setColor(0.55f, 0.75f, 1.0f, 0.06f + pulse * 0.04f);
        drawCircle(fx, fy, 60, 40);
    }

    setColor(0, 0, 0, 0.25f);
    drawCircle(fx + 3, fy + 4, 44, 40);

    setColor(0.20f, 0.20f, 0.24f);
    drawRect(fx - 16, fy + 54, 32, 14);
    setColor(0.40f, 0.40f, 0.48f);
    drawRectOutline(fx - 16, fy + 54, 32, 14, 1.2f);
    setColor(0.55f, 0.55f, 0.62f, 0.5f);
    drawRect(fx - 16, fy + 54, 32, 2);

    setColor(0.12f, 0.12f, 0.15f);
    drawCircle(fx - 10, fy + 61, 1.6f, 10);
    drawCircle(fx + 10, fy + 61, 1.6f, 10);
    setColor(0.4f, 0.4f, 0.45f, 0.6f);
    drawCircle(fx - 10.5f, fy + 60.5f, 0.6f, 6);
    drawCircle(fx + 9.5f, fy + 60.5f, 0.6f, 6);

    setColor(0.28f, 0.28f, 0.34f);
    drawRect(fx - 5, fy + 40, 10, 24);
    setColor(0.50f, 0.50f, 0.58f);
    drawRect(fx - 5, fy + 40, 2, 24);
    setColor(0.16f, 0.16f, 0.20f);
    drawRect(fx + 3, fy + 40, 2, 24);

    setColor(0.22f, 0.22f, 0.27f);
    drawCircle(fx, fy + 42, 9, 24);
    setColor(0.45f, 0.45f, 0.52f);
    drawCircleOutline(fx, fy + 42, 9, 24);
    setColor(0.65f, 0.68f, 0.75f, 0.8f);
    drawCircle(fx - 2.5f, fy + 39.5f, 2.2f, 12);

    setColor(0.18f, 0.18f, 0.23f);
    drawCircle(fx, fy, 19, 30);
    setColor(0.40f, 0.40f, 0.48f);
    drawCircleOutline(fx, fy, 19, 30);
    setColor(0.60f, 0.63f, 0.72f, 0.35f);
    drawCircleOutline(fx - 3, fy - 3, 14, 20);

    setColor(0.12f, 0.12f, 0.16f);
    for (int i = 0; i < 10; i++)
    {
        float a = i * 36.0f * M_PI / 180.0f;
        drawLine(fx + cos(a) * 11, fy + sin(a) * 11,
                 fx + cos(a) * 17, fy + sin(a) * 17, 1.3f);
    }

    setColor(0.55f, 0.55f, 0.62f, 0.9f);
    drawCircleOutline(fx, fy, 44, 55);
    setColor(0.70f, 0.72f, 0.80f, 0.6f);
    drawCircleOutline(fx, fy, 43, 55);
    setColor(0.38f, 0.38f, 0.45f);
    drawCircleOutline(fx, fy, 41, 50);
    drawCircleOutline(fx, fy, 25, 35);
    drawCircleOutline(fx, fy, 15, 28);

    setColor(1.0f, 1.0f, 1.0f, 0.18f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = -20; i <= 20; i++)
    {
        float a = (135.0f + i) * M_PI / 180.0f;
        glVertex2f(fx + cos(a) * 43, fy + sin(a) * 43);
    }
    glEnd();

    setColor(0.45f, 0.45f, 0.54f, 0.7f);
    glLineWidth(1.0f);
    for (int i = 0; i < 28; i++)
    {
        float a = i * (360.0f / 28.0f) * M_PI / 180.0f;
        drawLine(fx + cos(a) * 15, fy + sin(a) * 15,
                 fx + cos(a) * 42, fy + sin(a) * 42, 0.9f);
    }

    setColor(0.42f, 0.42f, 0.50f, 0.45f);
    drawCircleOutline(fx, fy, 20, 40);
    drawCircleOutline(fx, fy, 30, 46);
    drawCircleOutline(fx, fy, 36, 48);

    float rot = sensor.fanOn ? animTime * 576.0f : 0.0f;

    if (sensor.fanOn)
    {
        setColor(0.70f, 0.74f, 0.82f, 0.22f + pulse * 0.08f);
        drawCircle(fx, fy, 39, 50);
        setColor(0.80f, 0.84f, 0.92f, 0.14f);
        drawCircle(fx, fy, 36, 50);
        setColor(0.90f, 0.93f, 1.0f, 0.08f);
        drawCircle(fx, fy, 32, 50);
    }

    for (int i = 0; i < 3; i++)
    {
        float baseAngle = (rot + i * 120.0f) * M_PI / 180.0f;
        float bladeLen = 37;

        setColor(0.10f, 0.10f, 0.14f, 0.55f);
        glBegin(GL_POLYGON);
            glVertex2f(fx + cos(baseAngle - 0.25f) * 10, fy + sin(baseAngle - 0.25f) * 10);
            glVertex2f(fx + cos(baseAngle + 0.25f) * 10, fy + sin(baseAngle + 0.25f) * 10);
            glVertex2f(fx + cos(baseAngle + 0.45f) * (bladeLen * 0.7f), fy + sin(baseAngle + 0.45f) * (bladeLen * 0.7f));
            glVertex2f(fx + cos(baseAngle + 0.15f) * bladeLen, fy + sin(baseAngle + 0.15f) * bladeLen);
            glVertex2f(fx + cos(baseAngle - 0.05f) * bladeLen, fy + sin(baseAngle - 0.05f) * bladeLen);
            glVertex2f(fx + cos(baseAngle - 0.35f) * (bladeLen * 0.75f), fy + sin(baseAngle - 0.35f) * (bladeLen * 0.75f));
        glEnd();

        setColor(0.80f, 0.83f, 0.90f, 0.97f);
        glBegin(GL_POLYGON);
            glVertex2f(fx + cos(baseAngle - 0.22f) * 9, fy + sin(baseAngle - 0.22f) * 9);
            glVertex2f(fx + cos(baseAngle + 0.22f) * 9, fy + sin(baseAngle + 0.22f) * 9);
            glVertex2f(fx + cos(baseAngle + 0.42f) * (bladeLen * 0.7f), fy + sin(baseAngle + 0.42f) * (bladeLen * 0.7f));
            glVertex2f(fx + cos(baseAngle + 0.12f) * (bladeLen - 1), fy + sin(baseAngle + 0.12f) * (bladeLen - 1));
            glVertex2f(fx + cos(baseAngle - 0.02f) * (bladeLen - 1), fy + sin(baseAngle - 0.02f) * (bladeLen - 1));
            glVertex2f(fx + cos(baseAngle - 0.32f) * (bladeLen * 0.75f), fy + sin(baseAngle - 0.32f) * (bladeLen * 0.75f));
        glEnd();

        setColor(0.55f, 0.58f, 0.68f, 0.5f);
        glBegin(GL_POLYGON);
            glVertex2f(fx + cos(baseAngle - 0.30f) * (bladeLen * 0.75f), fy + sin(baseAngle - 0.30f) * (bladeLen * 0.75f));
            glVertex2f(fx + cos(baseAngle - 0.02f) * (bladeLen - 1), fy + sin(baseAngle - 0.02f) * (bladeLen - 1));
            glVertex2f(fx + cos(baseAngle + 0.08f) * (bladeLen - 3), fy + sin(baseAngle + 0.08f) * (bladeLen - 3));
            glVertex2f(fx + cos(baseAngle + 0.35f) * (bladeLen * 0.68f), fy + sin(baseAngle + 0.35f) * (bladeLen * 0.68f));
        glEnd();

        setColor(0.98f, 0.99f, 1.0f, 0.8f);
        glBegin(GL_POLYGON);
            glVertex2f(fx + cos(baseAngle - 0.15f) * 12, fy + sin(baseAngle - 0.15f) * 12);
            glVertex2f(fx + cos(baseAngle - 0.05f) * 12, fy + sin(baseAngle - 0.05f) * 12);
            glVertex2f(fx + cos(baseAngle - 0.02f) * (bladeLen - 3), fy + sin(baseAngle - 0.02f) * (bladeLen - 3));
            glVertex2f(fx + cos(baseAngle - 0.18f) * (bladeLen * 0.7f), fy + sin(baseAngle - 0.18f) * (bladeLen * 0.7f));
        glEnd();

        setColor(0.35f, 0.35f, 0.42f, 0.9f);
        glLineWidth(1);
        glBegin(GL_LINE_LOOP);
            glVertex2f(fx + cos(baseAngle - 0.22f) * 9, fy + sin(baseAngle - 0.22f) * 9);
            glVertex2f(fx + cos(baseAngle + 0.22f) * 9, fy + sin(baseAngle + 0.22f) * 9);
            glVertex2f(fx + cos(baseAngle + 0.42f) * (bladeLen * 0.7f), fy + sin(baseAngle + 0.42f) * (bladeLen * 0.7f));
            glVertex2f(fx + cos(baseAngle + 0.12f) * (bladeLen - 1), fy + sin(baseAngle + 0.12f) * (bladeLen - 1));
            glVertex2f(fx + cos(baseAngle - 0.02f) * (bladeLen - 1), fy + sin(baseAngle - 0.02f) * (bladeLen - 1));
            glVertex2f(fx + cos(baseAngle - 0.32f) * (bladeLen * 0.75f), fy + sin(baseAngle - 0.32f) * (bladeLen * 0.75f));
        glEnd();
    }

    setColor(0.14f, 0.14f, 0.18f);
    drawCircle(fx, fy, 13, 30);
    setColor(0.60f, 0.62f, 0.70f);
    drawCircle(fx, fy, 11, 30);
    setColor(0.38f, 0.38f, 0.48f);
    drawCircle(fx, fy, 8.5f, 26);
    setColor(0.12f, 0.12f, 0.15f);
    drawCircle(fx, fy, 3, 14);
    setColor(0.03f, 0.03f, 0.05f);
    drawLine(fx - 2.5f, fy, fx + 2.5f, fy, 1);
    drawLine(fx, fy - 2.5f, fx, fy + 2.5f, 1);
    setColor(0.95f, 0.95f, 1.0f, 0.85f);
    drawCircle(fx - 3.2f, fy - 3.2f, 2.3f, 12);
    setColor(0.8f, 0.6f, 0.2f, 0.7f);
    drawCircleOutline(fx, fy, 11.5f, 26);

    if (sensor.fanOn)
    {
        setColor(0.2f, 1.0f, 0.3f, 0.6f + pulse * 0.3f);
        drawCircle(fx + 32, fy + 32, 2.2f, 12);
        setColor(0.2f, 1.0f, 0.3f, 0.25f * pulse);
        drawCircle(fx + 32, fy + 32, 5, 18);
    }
    else
    {
        setColor(1.0f, 0.2f, 0.2f, 0.7f);
        drawCircle(fx + 32, fy + 32, 2.2f, 12);
    }

    if (sensor.fanOn)
    {
        setColor(0.05f, 0.05f, 0.08f, 0.4f);
        drawRect(fx - 29, fy + 97, 60, 18);
        setColor(0.08f, 0.35f, 0.14f, 0.92f);
        drawRect(fx - 30, fy + 95, 60, 18);
        setColor(0.3f, 1.0f, 0.45f, 0.9f);
        drawRectOutline(fx - 30, fy + 95, 60, 18, 1.5f);
        setColor(0.95f, 1.0f, 0.95f);
        drawText("FAN: ON", fx - 22, fy + 108);
    }
    else
    {
        setColor(0.05f, 0.05f, 0.06f, 0.4f);
        drawRect(fx - 29, fy + 97, 60, 18);
        setColor(0.22f, 0.22f, 0.26f, 0.92f);
        drawRect(fx - 30, fy + 95, 60, 18);
        setColor(0.5f, 0.5f, 0.56f);
        drawRectOutline(fx - 30, fy + 95, 60, 18, 1.5f);
        setColor(0.72f, 0.72f, 0.78f);
        drawText("FAN: OFF", fx - 24, fy + 108);
    }
}

void drawFan()
{
    drawSingleFan(115, 300);
    drawSingleFan(705, 300);
}

static void windFromSource(float srcX, float srcY, float seedOffset, int dir)
{
    for (int i = 0; i < 12; i++)
    {
        float t = fmod(animTime * 2.6f + i * 0.22f + seedOffset, 1.0f);
        float wy = srcY - 38 + i * 7;
        float wx = srcX + dir * (t * 290 - 65);
        float len = (26 + 32 * (1 - t)) * dir;

        setColor(0.85f, 0.92f, 1.0f, 0.42f * (1.0f - t));
        drawLine(wx, wy, wx - len, wy, 1.6f);
        setColor(0.97f, 0.99f, 1.0f, 0.20f * (1.0f - t));
        drawLine(wx, wy + 1, wx - len * 0.7f, wy + 1, 1);
    }

    for (int i = 0; i < 5; i++)
    {
        float t = fmod(animTime * 1.8f + i * 0.33f + seedOffset, 1.0f);
        float baseY = srcY - 22 + i * 18;
        float baseX = srcX + dir * (t * 255 - 62);

        setColor(0.82f, 0.91f, 1.0f, 0.26f * (1.0f - t));
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < 16; j++)
        {
            float px = baseX - dir * j * 4;
            float py = baseY + sin((j + animTime * 5) * 0.5f) * 3;
            glVertex2f(px, py);
        }
        glEnd();
    }

    for (int i = 0; i < 14; i++)
    {
        float t = fmod(animTime * 3.1f + i * 0.17f + seedOffset, 1.0f);
        float px = srcX + dir * (t * 265 - 62);
        float py = srcY - 27 + (i % 9) * 11 + sin(animTime * 4 + i) * 2;

        float alpha = 0.6f * (1.0f - t);
        setColor(0.92f, 0.96f, 1.0f, alpha);
        drawCircle(px, py, 1.5f * (1.0f - t * 0.5f), 8);
    }

    for (int i = 0; i < 3; i++)
    {
        float t = fmod(animTime * 1.4f + i * 0.28f + seedOffset, 1.0f);
        float radius = 20 + t * 44;
        float alpha = 0.15f * (1.0f - t);

        setColor(0.72f, 0.87f, 1.0f, alpha);
        drawCircleOutline(srcX + dir * t * 32, srcY, radius, 32);
    }
}

void drawWindEffect()
{
    if (!sensor.fanOn) return;

    windFromSource(115, 300, 0.0f, +1);
    windFromSource(705, 300, 0.5f, -1);
}