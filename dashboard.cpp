#include "dashboard.h"
#include "utils.h"

extern bool fanManualOverride;

static void gradientV(float x, float y, float w, float h, Color top, Color bottom)
{
    glBegin(GL_QUADS);
        glColor4f(top.r, top.g, top.b, top.a);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glColor4f(bottom.r, bottom.g, bottom.b, bottom.a);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd();
}

static void roundedPanel(float x, float y, float w, float h, float r, Color fill)
{
    setColor(fill);
    drawRect(x + r, y, w - 2 * r, h);
    drawRect(x, y + r, w, h - 2 * r);
    drawCircle(x + r, y + r, r, 16);
    drawCircle(x + w - r, y + r, r, 16);
    drawCircle(x + r, y + h - r, r, 16);
    drawCircle(x + w - r, y + h - r, r, 16);
}

static void sectionHeader(const std::string& title, float x, float y, float pulse)
{
    setColor(0.3f, 0.9f, 0.6f, 0.5f + pulse * 0.5f);
    drawCircle(x + 3, y - 4, 3, 10);
    setColor(0.59f, 0.78f, 0.59f);
    drawText(title, x + 12, y);
    setColor(0.25f, 0.45f, 0.35f, 0.6f);
    drawLine(x, y + 6, x + 300, y + 6, 1);
}

void drawDashboard()
{
    bool isDay = (dayTime > 0.2f && dayTime < 0.8f);
    float pulse = 0.5f + 0.5f * sin(animTime * 2.0f);

    setColor(0, 0, 0, 0.35f);
    drawRect(866, 16, 330, 680);

    roundedPanel(860, 10, 330, 680, 14, Color(0.07f, 0.10f, 0.16f, 0.96f));
    setColor(0.20f, 0.75f, 0.95f, 0.4f + pulse * 0.2f);
    drawRectOutline(860, 10, 330, 680, 2);
    setColor(0.24f, 0.39f, 0.55f);
    drawRectOutline(861, 11, 328, 678, 1);

    gradientV(862, 12, 326, 55, Color(0.10f, 0.30f, 0.50f, 1.0f), Color(0.06f, 0.16f, 0.28f, 1.0f));
    setColor(0.3f, 0.8f, 1.0f, 0.4f + pulse * 0.3f);
    drawLine(862, 67, 1188, 67, 2);

    setColor(0.39f, 0.86f, 1.0f);
    drawText("SMART GREENHOUSE", 895, 35);
    setColor(0.31f, 0.71f, 0.86f);
    drawText("CONTROL SYSTEM", 910, 55);

    setColor(0.3f, 1.0f, 0.5f, 0.6f + pulse * 0.4f);
    drawCircle(1165, 25, 4, 10);

    float y = 90;

    sectionHeader("SENSOR DATA", 880, y, pulse);
    y += 22;

    Color tempColor;
    if (sensor.temperature > 30) tempColor = Color(1.0f, 0.39f, 0.39f);
    else if (sensor.temperature > 22) tempColor = Color(0.39f, 0.86f, 0.39f);
    else tempColor = Color(0.39f, 0.71f, 1.0f);

    std::string tempStr = "Temp: " + floatToStr(sensor.temperature) + " C";
    drawProgressBar(875, y, 300, 26, sensor.temperature / 45.0f * 100,
                     tempColor, Color(0.16f, 0.16f, 0.24f),
                     Color(0.31f, 0.31f, 0.39f), tempStr);
    setColor(tempColor.r, tempColor.g, tempColor.b, 0.3f + pulse * 0.2f);
    drawRectOutline(875, y, 300, 26, 2);
    y += 36;

    Color moistColor;
    if (sensor.soilMoisture < 30) moistColor = Color(1.0f, 0.71f, 0.20f);
    else if (sensor.soilMoisture > 70) moistColor = Color(0.39f, 0.59f, 1.0f);
    else moistColor = Color(0.39f, 0.86f, 0.39f);

    std::string moistStr = "Moisture: " + floatToStr(sensor.soilMoisture) + " %";
    drawProgressBar(875, y, 300, 26, sensor.soilMoisture,
                     moistColor, Color(0.16f, 0.16f, 0.24f),
                     Color(0.31f, 0.31f, 0.39f), moistStr);
    setColor(moistColor.r, moistColor.g, moistColor.b, 0.3f + pulse * 0.2f);
    drawRectOutline(875, y, 300, 26, 2);
    y += 36;

    std::string lightStr = "Light: " + floatToStr(sensor.lightLevel) + " %";
    drawProgressBar(875, y, 300, 26, sensor.lightLevel,
                     Color(1.0f, 0.90f, 0.31f),
                     Color(0.16f, 0.16f, 0.24f),
                     Color(0.31f, 0.31f, 0.39f), lightStr);
    setColor(1.0f, 0.90f, 0.31f, 0.3f + pulse * 0.2f);
    drawRectOutline(875, y, 300, 26, 2);
    y += 48;

    sectionHeader("DEVICE STATUS", 880, y, pulse);
    y += 22;

    struct DevInfo {
        std::string name;
        std::string reason;
        bool on;
        Color onColor;
    };

    DevInfo devices[3] = {
        {"FAN",        fanManualOverride ? "Manual Mode" : "Cooling: Temp > 30C",  sensor.fanOn,       Color(0.20f, 0.45f, 0.85f)},
        {"SPRINKLER",  "Watering: Soil < 35%", sensor.sprinklerOn, Color(0.12f, 0.55f, 0.80f)},
        {"LED LIGHTS", "Low Light < 30%",      sensor.ledOn,       Color(0.85f, 0.65f, 0.10f)}
    };

    for (int i = 0; i < 3; i++)
    {
        DevInfo& d = devices[i];

        if (d.on)
        {
            setColor(d.onColor.r, d.onColor.g, d.onColor.b, 0.20f + pulse * 0.15f);
            drawRect(872, y - 3, 296, 52);
        }

        Color cardColor = d.on
            ? Color(d.onColor.r * 0.30f, d.onColor.g * 0.30f, d.onColor.b * 0.40f, 0.92f)
            : Color(0.14f, 0.14f, 0.18f, 0.92f);

        roundedPanel(875, y, 290, 46, 8, cardColor);

        Color borderColor = d.on ? Color(1.0f, 1.0f, 1.0f, 0.6f) : Color(0.35f, 0.35f, 0.40f, 0.7f);
        setColor(borderColor);
        drawRectOutline(875, y, 290, 46, 1.5f);

        Color dotColor = d.on ? d.onColor : Color(0.3f, 0.3f, 0.3f);

        setColor(dotColor.r, dotColor.g, dotColor.b, 0.25f);
        drawCircle(890, y + 23, d.on ? (12 + pulse * 2) : 10, 16);

        setColor(dotColor.r, dotColor.g, dotColor.b, 1.0f);
        drawCircle(890, y + 23, 7, 14);

        setColor(1, 1, 1, 0.5f);
        drawCircle(888, y + 21, 2, 8);

        setColor(1, 1, 1);
        drawText(d.name, 908, y + 18);

        Color subColor = d.on ? Color(0.86f, 1.0f, 0.86f) : Color(0.5f, 0.5f, 0.55f);
        setColor(subColor);
        drawText(d.on ? d.reason : "Standby", 908, y + 36);

        // Manual/Auto badge for Fan
        if (i == 0 && fanManualOverride)
        {
            setColor(1.0f, 0.85f, 0.2f, 0.9f);
            drawCircle(1100, y + 10, 3, 10);
        }

        Color badgeColor = d.on ? Color(0.05f, 0.65f, 0.3f) : Color(0.45f, 0.18f, 0.18f);
        roundedPanel(1112, y + 13, 48, 21, 5, badgeColor);
        setColor(1, 1, 1, 0.3f);
        drawRectOutline(1112, y + 13, 48, 21, 1);
        setColor(1, 1, 1);
        drawText(d.on ? " ON" : "OFF", 1121, y + 28);

        y += 54;
    }

    y += 6;

    sectionHeader("ENVIRONMENT", 880, y, pulse);
    y += 22;

    roundedPanel(875, y, 290, 84, 10, Color(0.08f, 0.14f, 0.22f, 0.92f));
    setColor(0.30f, 0.45f, 0.60f, 0.6f);
    drawRectOutline(875, y, 290, 84, 1.5f);

    if (isDay)
    {
        setColor(1.0f, 0.85f, 0.3f, 0.9f);
        drawCircle(895, y + 20, 8, 16);
        for (int i = 0; i < 8; i++)
        {
            float a = i * 45.0f * M_PI / 180.0f;
            drawLine(895 + cos(a) * 10, y + 20 + sin(a) * 10,
                     895 + cos(a) * 14, y + 20 + sin(a) * 14, 1.5f);
        }
    }
    else
    {
        setColor(0.6f, 0.65f, 0.9f, 0.9f);
        drawCircle(895, y + 20, 8, 16);
        setColor(0.08f, 0.14f, 0.22f, 1.0f);
        drawCircle(899, y + 17, 7, 16);
    }

    Color dayColor = isDay ? Color(1.0f, 0.86f, 0.31f) : Color(0.6f, 0.65f, 0.9f);
    setColor(dayColor);
    drawText(std::string("Status:  ") + (isDay ? "DAY TIME" : "NIGHT TIME"), 910, y + 24);

    if (isRaining)
    {
        setColor(0.5f, 0.6f, 0.7f, 0.9f);
        drawCircle(895, y + 44, 7, 14);
        setColor(0.4f, 0.7f, 1.0f, 0.8f);
        drawLine(891, y + 48, 889, y + 54, 1.5f);
        drawLine(895, y + 48, 893, y + 54, 1.5f);
        drawLine(899, y + 48, 897, y + 54, 1.5f);
    }
    else
    {
        setColor(1.0f, 0.85f, 0.3f, 0.9f);
        drawCircle(895, y + 44, 6, 14);
    }

    Color weatherColor = isRaining ? Color(0.39f, 0.71f, 1.0f) : Color(0.71f, 0.71f, 0.71f);
    setColor(weatherColor);
    drawText(std::string("Weather: ") + (isRaining ? "RAINING" : "CLEAR"), 910, y + 48);

    float humidity = sensor.soilMoisture * 0.8f + 20;
    setColor(0.71f, 0.86f, 1.0f);
    drawText("Humidity: " + floatToStr(humidity) + " %", 910, y + 70);

    y += 98;

    sectionHeader("AUTO LOG", 880, y, pulse);
    y += 22;

    int logCount = 0;

    if (sensor.fanOn)
    {
        setColor(0.59f, 0.71f, 1.0f, 0.9f);
        drawRect(876, y - 8, 3, 13);
        setColor(0.59f, 0.71f, 1.0f);
        drawText(fanManualOverride ? "[MANUAL] Fan ON" : "[AUTO] Fan ON - Cooling", 884, y);
        y += 18;
        logCount++;
    }
    if (sensor.sprinklerOn)
    {
        setColor(0.39f, 0.78f, 1.0f, 0.9f);
        drawRect(876, y - 8, 3, 13);
        setColor(0.39f, 0.78f, 1.0f);
        drawText("[AUTO] Sprinkler ON - Watering", 884, y);
        y += 18;
        logCount++;
    }
    if (sensor.ledOn)
    {
        setColor(1.0f, 0.90f, 0.39f, 0.9f);
        drawRect(876, y - 8, 3, 13);
        setColor(1.0f, 0.90f, 0.39f);
        drawText("[AUTO] LEDs ON - Lighting", 884, y);
        y += 18;
        logCount++;
    }
    if (logCount == 0)
    {
        setColor(0.39f, 0.86f, 0.51f, 0.9f);
        drawRect(876, y - 8, 3, 13);
        setColor(0.39f, 0.86f, 0.51f);
        drawText("[INFO] All systems nominal", 884, y);
        y += 18;
    }

    y += 10;

    sectionHeader("PLANT GROWTH", 880, y, pulse);
    y += 22;

    for (int i = 0; i < (int)plants.size() && i < 4; i++)
    {
        float pct = plants[i].growth * 100;
        std::string stage;
        Color scolor;

        if (pct < 25)      { stage = "Seed";    scolor = Color(0.71f, 0.55f, 0.31f); }
        else if (pct < 50) { stage = "Sprout";  scolor = Color(0.59f, 0.86f, 0.39f); }
        else if (pct < 75) { stage = "Sapling"; scolor = Color(0.31f, 0.71f, 0.31f); }
        else if (pct < 90) { stage = "Mature";  scolor = Color(0.20f, 0.59f, 0.20f); }
        else               { stage = "Bloom";   scolor = Color(1.0f, 0.59f, 0.78f); }

        std::string label = "P" + std::to_string(i + 1) + ": " + stage + " " +
                             floatToStr(pct, 0) + "%";
        drawProgressBar(875, y, 290, 20, pct,
                         scolor, Color(0.12f, 0.16f, 0.22f),
                         Color(0.27f, 0.35f, 0.43f), label);

        if (pct >= 90)
        {
            setColor(scolor.r, scolor.g, scolor.b, 0.3f + pulse * 0.3f);
            drawRectOutline(875, y, 290, 20, 2);
        }

        y += 26;
    }
}

void drawTopBar()
{
    float pulse = 0.5f + 0.5f * sin(animTime * 2.5f);

    gradientV(0, 0, WIN_W, 32, Color(0.06f, 0.14f, 0.24f, 0.95f), Color(0.03f, 0.08f, 0.15f, 0.95f));
    setColor(0.2f, 0.6f, 0.85f, 0.3f + pulse * 0.2f);
    drawLine(0, 32, WIN_W, 32, 1.5f);

    setColor(0.47f, 0.78f, 0.86f);
    drawText("SMART GREENHOUSE AUTOMATION SYSTEM", 10, 21);

    gradientV(WIN_W - 700, 0, 700, 32, Color(0.12f, 0.24f, 0.40f, 0.95f), Color(0.07f, 0.16f, 0.28f, 0.95f));
    setColor(0.35f, 0.65f, 0.85f, 0.7f);
    drawRectOutline(WIN_W - 700, 0, 700, 32, 1);

    setColor(1.0f, 0.85f, 0.3f);
    drawText("CONTROLS:", WIN_W - 690, 21);

    float bx = WIN_W - 600;
    float by = 5;
    float bw = 90;
    float bh = 22;

    gradientV(bx, by, bw, bh, Color(0.26f, 0.42f, 0.63f), Color(0.16f, 0.28f, 0.47f));
    setColor(0.50f, 0.75f, 1.0f);
    drawRectOutline(bx, by, bw, bh, 1);
    setColor(1, 1, 1);
    drawText("Arrows: Temp", bx + 5, by + 15);

    bx += bw + 8;
    gradientV(bx, by, bw, bh, Color(0.26f, 0.42f, 0.63f), Color(0.16f, 0.28f, 0.47f));
    setColor(0.50f, 0.75f, 1.0f);
    drawRectOutline(bx, by, bw, bh, 1);
    setColor(1, 1, 1);
    drawText("<-/-> Moist", bx + 5, by + 15);

    bx += bw + 8;
    Color rTop = isRaining ? Color(0.26f, 0.55f, 0.85f) : Color(0.40f, 0.40f, 0.45f);
    Color rBot = isRaining ? Color(0.14f, 0.35f, 0.65f) : Color(0.26f, 0.26f, 0.30f);
    gradientV(bx, by, 55, bh, rTop, rBot);
    setColor(0.50f, 0.80f, 1.0f);
    drawRectOutline(bx, by, 55, bh, 1);
    if (isRaining)
    {
        setColor(0.5f, 0.8f, 1.0f, 0.3f + pulse * 0.3f);
        drawRectOutline(bx - 1, by - 1, 57, bh + 2, 2);
    }
    setColor(1, 1, 1);
    drawText("R: Rain", bx + 5, by + 15);

    bx += 55 + 8;
    gradientV(bx, by, 45, bh, Color(0.65f, 0.55f, 0.25f), Color(0.45f, 0.35f, 0.15f));
    setColor(1.0f, 0.85f, 0.3f);
    drawRectOutline(bx, by, 45, bh, 1);
    setColor(1, 1, 1);
    drawText("D:Fast", bx + 3, by + 15);

    bx += 45 + 8;
    gradientV(bx, by, 45, bh, Color(0.38f, 0.38f, 0.65f), Color(0.22f, 0.22f, 0.45f));
    setColor(0.6f, 0.6f, 1.0f);
    drawRectOutline(bx, by, 45, bh, 1);
    setColor(1, 1, 1);
    drawText("N:Slow", bx + 3, by + 15);

    // ---- FAN CONTROL BUTTON ----
    bx += 45 + 8;
    Color fTop = sensor.fanOn ? Color(0.20f, 0.55f, 0.85f) : Color(0.35f, 0.35f, 0.40f);
    Color fBot = sensor.fanOn ? Color(0.10f, 0.35f, 0.60f) : Color(0.22f, 0.22f, 0.26f);
    gradientV(bx, by, 55, bh, fTop, fBot);
    setColor(0.50f, 0.80f, 1.0f);
    drawRectOutline(bx, by, 55, bh, 1);
    if (sensor.fanOn)
    {
        float pulse2 = 0.5f + 0.5f * sin(animTime * 3.0f);
        setColor(0.5f, 0.85f, 1.0f, 0.3f + pulse2 * 0.3f);
        drawRectOutline(bx - 1, by - 1, 57, bh + 2, 2);
    }
    setColor(1, 1, 1);
    drawText("F: Fan", bx + 5, by + 15);
    if (fanManualOverride)
    {
        setColor(1.0f, 0.85f, 0.2f, 0.9f);
        drawCircle(bx + 50, by + 3, 3, 8);
    }

    // ---- A: AUTO MODE BUTTON ----
    bx += 55 + 8;
    Color aTop = fanManualOverride ? Color(0.45f, 0.35f, 0.15f) : Color(0.20f, 0.55f, 0.35f);
    Color aBot = fanManualOverride ? Color(0.30f, 0.22f, 0.08f) : Color(0.10f, 0.35f, 0.20f);
    gradientV(bx, by, 45, bh, aTop, aBot);
    setColor(0.60f, 0.90f, 0.70f);
    drawRectOutline(bx, by, 45, bh, 1);
    setColor(1, 1, 1);
    drawText("A:Auto", bx + 3, by + 15);

    // ---- ESC ----
    bx += 45 + 8;
    gradientV(bx, by, 55, bh, Color(0.75f, 0.25f, 0.25f), Color(0.50f, 0.15f, 0.15f));
    setColor(1.0f, 0.40f, 0.40f);
    drawRectOutline(bx, by, 55, bh, 1);
    setColor(1, 1, 1);
    drawText("ESC:Exit", bx + 5, by + 15);
}

void drawBottomBar()
{
    gradientV(0, WIN_H - 28, WIN_W, 28, Color(0.03f, 0.08f, 0.15f, 0.92f), Color(0.06f, 0.13f, 0.22f, 0.92f));
    setColor(0.2f, 0.6f, 0.85f, 0.4f);
    drawLine(0, WIN_H - 28, WIN_W, WIN_H - 28, 1.2f);

    std::string status =
        " Temp: " + floatToStr(sensor.temperature) + "C  |  "
        "Moist: " + floatToStr(sensor.soilMoisture) + "%  |  "
        "Light: " + floatToStr(sensor.lightLevel) + "%  |  " +
        std::string((dayTime > 0.2f && dayTime < 0.8f) ? "DAY" : "NIGHT") + "  |  " +
        std::string(isRaining ? "RAIN" : "CLEAR") + "  |  "
        "Fan: " + std::string(sensor.fanOn ? "ON" : "OFF") +
        (fanManualOverride ? "(M)" : "(A)") + "  |  "
        "Sprinkler: " + std::string(sensor.sprinklerOn ? "ON" : "OFF") + "  |  "
        "LED: " + std::string(sensor.ledOn ? "ON" : "OFF") + "  |  "
        "Plants: " + std::to_string(plants.size());

    setColor(0.65f, 0.85f, 0.85f);
    drawText(status, 8, WIN_H - 10);
}