#include "utils.h"

void setColor(float r, float g, float b, float a)
{
    glColor4f(r, g, b, a);
}

void setColor(Color c)
{
    glColor4f(c.r, c.g, c.b, c.a);
}

void drawRect(float x, float y, float w, float h)
{
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd();
}

void drawRectOutline(float x, float y, float w, float h, float lineWidth)
{
    glLineWidth(lineWidth);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd();
}

void drawFilledRect(float x, float y, float w, float h, Color fill, Color outline, float lineW)
{
    setColor(fill);
    drawRect(x, y, w, h);
    setColor(outline);
    drawRectOutline(x, y, w, h, lineW);
}

void drawCircle(float cx, float cy, float radius, int segments)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        glVertex2f(cx + cos(angle) * radius, cy + sin(angle) * radius);
    }
    glEnd();
}

void drawCircleOutline(float cx, float cy, float radius, int segments)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++) {
        float angle = 2.0f * M_PI * i / segments;
        glVertex2f(cx + cos(angle) * radius, cy + sin(angle) * radius);
    }
    glEnd();
}

void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
    glBegin(GL_TRIANGLES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
    glEnd();
}

void drawLine(float x1, float y1, float x2, float y2, float lineWidth)
{
    glLineWidth(lineWidth);
    glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    glEnd();
}

void drawGradientRect(float x, float y, float w, float h, Color topColor, Color bottomColor)
{
    glBegin(GL_QUADS);
        glColor4f(topColor.r, topColor.g, topColor.b, topColor.a);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glColor4f(bottomColor.r, bottomColor.g, bottomColor.b, bottomColor.a);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd();
}

// ============================================================
//  TEXT RENDERING (Windows Bitmap Font)
// ============================================================
void drawText(const std::string& text, float x, float y)
{
    if (fontBase == 0) return;

    glRasterPos2f(x, y);
    glPushAttrib(GL_LIST_BIT);
        glListBase(fontBase - 32);
        glCallLists(text.length(), GL_UNSIGNED_BYTE, text.c_str());
    glPopAttrib();
}

void drawTextLarge(const std::string& text, float x, float y)
{
    // Same font, but you can create separate large font later
    drawText(text, x, y);
}

void drawTextSmall(const std::string& text, float x, float y)
{
    drawText(text, x, y);
}

// ============================================================
//  PROGRESS BAR
// ============================================================
void drawProgressBar(float x, float y, float w, float h,
                      float percent, Color fill, Color bg, Color border,
                      const std::string& label)
{
    percent = clamp(percent, 0, 100);

    setColor(bg);
    drawRect(x, y, w, h);

    float fw = (w - 4) * (percent / 100.0f);
    setColor(fill);
    drawRect(x + 2, y + 2, fw, h - 4);

    setColor(border);
    drawRectOutline(x, y, w, h, 2);

    setColor(1, 1, 1);
    drawText(label, x + 5, y + h * 0.65f);
}

float lerp(float a, float b, float t)   { return a + (b - a) * t; }

float clamp(float val, float minV, float maxV)
{
    if (val < minV) return minV;
    if (val > maxV) return maxV;
    return val;
}

std::string floatToStr(float val, int precision)
{
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(precision) << val;
    return ss.str();
}