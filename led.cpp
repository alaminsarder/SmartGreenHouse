#include "led.h"
#include "utils.h"

// ============================================================
//  REALISTIC LED GROW LIGHTS
// ============================================================
void drawLEDs()
{
    int numLED = 12;

    for (int i = 0; i < numLED; i++)
    {
        float lx = 100.0f + i * 55.0f;
        float ly = 270.0f;

        // ====================================================
        // HANGING WIRE
        // ====================================================
        setColor(0.12f, 0.12f, 0.14f);
        drawLine(lx, ly - 28, lx, ly - 7, 2.0f);

        // Small wire connector
        setColor(0.22f, 0.22f, 0.24f);
        drawCircle(lx, ly - 7, 3.5f, 10);

        // ====================================================
        // LIGHT ON
        // ====================================================
        if (sensor.ledOn)
        {
            float pulse =
                0.88f + 0.12f * sin(animTime * 2.5f + i * 0.35f);

            // ------------------------------------------------
            // SOFT LIGHT SPILL / GLOW
            // ------------------------------------------------
            setColor(1.0f, 0.92f, 0.55f, 0.025f * pulse);
            drawCircle(lx, ly + 12, 48, 30);

            setColor(1.0f, 0.95f, 0.65f, 0.045f * pulse);
            drawCircle(lx, ly + 10, 34, 28);

            setColor(1.0f, 0.96f, 0.72f, 0.07f * pulse);
            drawCircle(lx, ly + 8, 23, 24);

            // ------------------------------------------------
            // REALISTIC DOWNWARD LIGHT BEAM
            // ------------------------------------------------
            glBegin(GL_TRIANGLES);

                setColor(1.0f, 0.95f, 0.65f, 0.025f * pulse);
                glVertex2f(lx - 7, ly + 4);
                glVertex2f(lx - 48, ly + 185);
                glVertex2f(lx + 48, ly + 185);

            glEnd();

            // Second softer beam
            glBegin(GL_TRIANGLES);

                setColor(1.0f, 0.98f, 0.75f, 0.018f * pulse);
                glVertex2f(lx - 3, ly + 5);
                glVertex2f(lx - 70, ly + 210);
                glVertex2f(lx + 70, ly + 210);

            glEnd();

            // =================================================
            // METAL LED FIXTURE / HOUSING
            // =================================================

            // Top dark housing
            setColor(0.13f, 0.14f, 0.16f);
            drawCircle(lx, ly, 13, 24);

            // Outer metallic ring
            setColor(0.32f, 0.34f, 0.37f);
            drawCircle(lx, ly, 11.5f, 24);

            // Reflector
            setColor(0.60f, 0.62f, 0.65f);
            drawCircle(lx, ly + 1, 9.5f, 24);

            // Reflector shadow
            setColor(0.38f, 0.40f, 0.43f);
            drawCircle(lx, ly + 2, 8.0f, 24);

            // =================================================
            // LED DIFFUSER
            // =================================================
            setColor(1.0f, 0.96f, 0.70f, 0.95f);
            drawCircle(lx, ly + 2, 6.8f, 24);

            // Bright diffuser center
            setColor(1.0f, 1.0f, 0.88f, 1.0f);
            drawCircle(lx, ly + 2, 4.5f, 20);

            // =================================================
            // INDIVIDUAL LED CHIP DETAIL
            // =================================================
            setColor(1.0f, 1.0f, 0.96f, 0.95f);

            drawCircle(lx - 2.2f, ly + 3.0f, 1.4f, 10);
            drawCircle(lx + 2.2f, ly + 3.0f, 1.4f, 10);
            drawCircle(lx, ly + 0.3f, 1.3f, 10);

            // =================================================
            // SMALL SPECULAR HIGHLIGHT
            // =================================================
            setColor(1.0f, 1.0f, 1.0f, 0.85f);
            drawCircle(lx - 4.0f, ly - 2.0f, 1.7f, 10);

            // =================================================
            // OUTER FIXTURE RIM
            // =================================================
            setColor(0.20f, 0.21f, 0.23f);
            drawCircleOutline(lx, ly, 12.5f, 24);

            // Inner metallic rim
            setColor(0.52f, 0.54f, 0.57f);
            drawCircleOutline(lx, ly + 1, 9.2f, 24);

            // Bottom rim
            setColor(0.15f, 0.16f, 0.18f);
            drawLine(lx - 7, ly + 7, lx + 7, ly + 7, 2.0f);
        }

        // ====================================================
        // LED OFF
        // ====================================================
        else
        {
            // Dark metal housing
            setColor(0.12f, 0.13f, 0.15f);
            drawCircle(lx, ly, 13, 24);

            // Metallic reflector
            setColor(0.28f, 0.29f, 0.32f);
            drawCircle(lx, ly + 1, 9.5f, 24);

            // Dark diffuser
            setColor(0.22f, 0.22f, 0.20f);
            drawCircle(lx, ly + 2, 6.5f, 24);

            // Small inactive LED chips
            setColor(0.30f, 0.30f, 0.27f);
            drawCircle(lx - 2.2f, ly + 3.0f, 1.3f, 10);
            drawCircle(lx + 2.2f, ly + 3.0f, 1.3f, 10);
            drawCircle(lx, ly + 0.3f, 1.2f, 10);

            // Outer rim
            setColor(0.35f, 0.36f, 0.39f);
            drawCircleOutline(lx, ly, 12.5f, 24);

            // Inner rim
            setColor(0.46f, 0.47f, 0.49f);
            drawCircleOutline(lx, ly + 1, 9.2f, 24);

            // Bottom rim
            setColor(0.13f, 0.14f, 0.16f);
            drawLine(lx - 7, ly + 7, lx + 7, ly + 7, 2.0f);
        }
    }
}