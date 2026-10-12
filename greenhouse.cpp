#include "greenhouse.h"
#include "utils.h"

void drawSoilBeds()
{
    drawFilledRect(60, 508, 770, 40,
                    Color(0.40f, 0.26f, 0.13f),
                    Color(0.31f, 0.20f, 0.08f), 2);

    drawFilledRect(80, 485, 720, 25,
                    Color(0.45f, 0.28f, 0.14f),
                    Color(0.31f, 0.20f, 0.08f), 2);

    setColor(0.35f, 0.22f, 0.10f);
    for (int i = 0; i < 40; i++)
    {
        float dx = 70 + (i % 20) * 40;
        float dy = 518 + (i / 20) * 15;
        drawCircle(dx, dy, 2, 8);
    }

    setColor(0.55f, 0.45f, 0.35f);
    for (int i = 0; i < 15; i++)
    {
        float dx = 80 + (i % 8) * 95;
        float dy = 512 + (i / 8) * 30;
        drawCircle(dx, dy, 1.5f, 6);
    }

    setColor(0.35f, 0.22f, 0.10f);
    for (int i = 0; i < 20; i++)
    {
        float dx = 90 + (i % 15) * 50;
        float dy = 495 + (i / 15) * 10;
        drawCircle(dx, dy, 1.5f, 8);
    }
}

void drawGreenhouseFloor()
{
    setColor(0.31f, 0.47f, 0.23f);
    drawRect(50, 540, 750, 120);

    drawFilledRect(50, 548, 750, 12,
                    Color(0.71f, 0.63f, 0.47f),
                    Color(0.55f, 0.47f, 0.35f), 1);

    setColor(0.65f, 0.57f, 0.42f);
    for (int i = 0; i < 10; i++)
    {
        drawLine(50 + i * 75, 548, 50 + i * 75, 560, 1);
    }
}

void drawGreenhouseFoundation()
{
    drawFilledRect(45, 540, 760, 10,
                    Color(0.55f, 0.55f, 0.52f),
                    Color(0.35f, 0.35f, 0.33f), 2);

    setColor(0.40f, 0.40f, 0.38f, 0.6f);
    for (int i = 0; i < 20; i++)
        drawLine(45 + i * 38, 540, 45 + i * 38, 550, 1);

    setColor(0.45f, 0.45f, 0.42f);
    drawRect(45, 538, 20, 14);
    drawRect(785, 538, 20, 14);
}

void drawGreenhouseWalls()
{
    drawFilledRect(50, 248, 16, 297,
                    Color(0.75f, 0.88f, 1.0f, 0.5f),
                    Color(0.30f, 0.55f, 0.70f), 2);

    drawFilledRect(784, 248, 16, 297,
                    Color(0.75f, 0.88f, 1.0f, 0.5f),
                    Color(0.30f, 0.55f, 0.70f), 2);

    drawFilledRect(65, 505, 720, 40,
                    Color(0.55f, 0.58f, 0.60f),
                    Color(0.35f, 0.38f, 0.40f), 2);

    setColor(0.45f, 0.48f, 0.50f, 0.5f);
    for (int i = 0; i < 9; i++)
        drawLine(65 + i * 90, 505, 65 + i * 90, 545, 1);

    int rows = 2;
    float panelTop = 255;
    float panelBottom = 505;
    float rowH = (panelBottom - panelTop) / rows;

    for (int r = 0; r < rows; r++)
    {
        float py = panelTop + r * rowH;
        setColor(0.80f, 0.93f, 1.0f, 0.18f);
        drawRect(66, py, 718, rowH - 4);
        setColor(0.55f, 0.78f, 0.90f, 0.5f);
        drawRectOutline(66, py, 718, rowH - 4, 1.2f);

        setColor(1, 1, 1, 0.10f);
        drawLine(66, py + rowH * 0.25f, 784, py + rowH * 0.15f, 1);
        drawLine(66, py + rowH * 0.65f, 784, py + rowH * 0.55f, 1);
    }

    setColor(0.40f, 0.62f, 0.75f, 0.85f);
    for (int i = 0; i <= 8; i++)
    {
        float mx = 66 + i * 90;
        drawLine(mx, panelTop, mx, panelBottom, 1.5f);
    }
    drawLine(66, panelTop + rowH, 784, panelTop + rowH, 1.5f);
}

void drawGreenhouseRoof()
{
    setColor(0.75f, 0.90f, 1.0f, 0.35f);
    drawTriangle(400, 100, 30, 255, 400, 255);
    setColor(0.68f, 0.85f, 0.98f, 0.35f);
    drawTriangle(400, 100, 400, 255, 770, 255);

    setColor(0.30f, 0.55f, 0.70f);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
        glVertex2f(400, 100);
        glVertex2f(30, 255);
        glVertex2f(770, 255);
    glEnd();

    setColor(0.35f, 0.60f, 0.75f, 0.75f);
    int ribs = 7;
    for (int i = 1; i <= ribs; i++)
    {
        float t = (float)i / (ribs + 1);
        float lx = 30 + t * (400 - 30);
        float ly = 255 + t * (100 - 255);
        drawLine(lx, ly, 400, 100, 1.4f);

        float rx = 770 - t * (770 - 400);
        float ry = 255 + t * (100 - 255);
        drawLine(rx, ry, 400, 100, 1.4f);
    }

    setColor(0.35f, 0.60f, 0.75f, 0.55f);
    for (int k = 1; k <= 2; k++)
    {
        float t = k / 3.0f;
        float lxA = 30 + t * (400 - 30);
        float lyA = 255 + t * (100 - 255);
        float rxA = 770 - t * (770 - 400);
        drawLine(lxA, lyA, rxA, lyA, 1.3f);
    }

    setColor(0.28f, 0.40f, 0.48f);
    drawRect(385, 96, 30, 8);
    setColor(0.45f, 0.60f, 0.70f);
    drawRectOutline(385, 96, 30, 8, 1);

    drawFilledRect(380, 80, 40, 18,
                    Color(0.55f, 0.75f, 0.85f, 0.5f),
                    Color(0.30f, 0.50f, 0.62f), 1.5f);
    setColor(0.30f, 0.50f, 0.62f);
    drawTriangle(380, 80, 400, 68, 420, 80);

    setColor(0.25f, 0.40f, 0.48f, 0.7f);
    for (int i = 0; i < 3; i++)
        drawLine(383, 85 + i * 4, 417, 85 + i * 4, 1);

    setColor(0.40f, 0.40f, 0.42f);
    drawRect(26, 253, 8, 6);
    drawRect(766, 253, 8, 6);
    setColor(0.55f, 0.55f, 0.58f);
    drawRectOutline(26, 253, 8, 6, 0.8f);
    drawRectOutline(766, 253, 8, 6, 0.8f);

    setColor(0.45f, 0.45f, 0.48f);
    drawRect(27, 259, 4, 286);
    drawRect(769, 259, 4, 286);
}

void drawGreenhouseDoor()
{
    setColor(0.30f, 0.50f, 0.62f, 0.7f);
    drawTriangle(355, 430, 400, 410, 445, 430);
    setColor(0.20f, 0.38f, 0.48f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(355, 430);
        glVertex2f(400, 410);
        glVertex2f(445, 430);
    glEnd();

    drawFilledRect(364, 434, 72, 112,
                    Color(0.38f, 0.26f, 0.14f),
                    Color(0.25f, 0.16f, 0.08f), 2);

    drawFilledRect(368, 438, 64, 104,
                    Color(0.52f, 0.36f, 0.19f),
                    Color(0.35f, 0.22f, 0.10f), 2);

    drawFilledRect(370, 440, 60, 100,
                    Color(0.62f, 0.42f, 0.22f),
                    Color(0.39f, 0.27f, 0.12f), 1);

    setColor(0.45f, 0.30f, 0.15f, 0.6f);
    for (int i = 1; i < 4; i++)
        drawLine(370, 440 + i * 25, 430, 440 + i * 25, 1);

    setColor(0.7f, 0.85f, 1.0f, 0.45f);
    drawRect(380, 450, 40, 30);
    setColor(0.5f, 0.7f, 0.9f);
    drawRectOutline(380, 450, 40, 30, 1.2f);
    drawLine(400, 450, 400, 480, 1);
    drawLine(380, 465, 420, 465, 1);
    setColor(1, 1, 1, 0.15f);
    drawLine(382, 478, 398, 452, 1);

    setColor(0.86f, 0.71f, 0.20f);
    drawCircle(395, 500, 5, 12);
    setColor(0.65f, 0.52f, 0.12f);
    drawCircleOutline(395, 500, 5, 12);
    setColor(1.0f, 0.95f, 0.7f, 0.6f);
    drawCircle(393, 498, 1.5f, 8);

    drawFilledRect(360, 544, 80, 8,
                    Color(0.55f, 0.55f, 0.52f),
                    Color(0.35f, 0.35f, 0.33f), 1.5f);
}

void drawGreenhouseColumns()
{
    for (int i = 0; i < 5; i++)
    {
        float cx = 65 + i * 180;

        drawFilledRect(cx, 250, 10, 295,
                        Color(0.62f, 0.78f, 0.85f, 0.75f),
                        Color(0.35f, 0.58f, 0.72f), 1.2f);

        setColor(0.40f, 0.62f, 0.75f);
        drawRect(cx - 3, 248, 16, 6);

        setColor(0.40f, 0.62f, 0.75f);
        drawRect(cx - 3, 540, 16, 8);

        if (i < 4)
        {
            float nx = 65 + (i + 1) * 180;
            setColor(0.35f, 0.58f, 0.72f, 0.55f);
            drawLine(cx + 5, 280, nx + 5, 320, 1.3f);
            drawLine(cx + 5, 320, nx + 5, 280, 1.3f);
        }
    }
}

void drawGreenhouse()
{
    drawGreenhouseFloor();
    drawGreenhouseFoundation();
    drawSoilBeds();
    drawGreenhouseWalls();
    drawGreenhouseRoof();
    drawGreenhouseDoor();
    drawGreenhouseColumns();
}