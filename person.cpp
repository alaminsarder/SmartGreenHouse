#include "person.h"
#include "utils.h"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>

enum PersonState { WAITING, WALKING };

struct Person
{
    float x, y;
    float baseSpeed;
    float speedFactor;   // randomized each time walking starts
    int dir;
    float walkPhase;
    Color shirtColor;
    Color pantColor;
    Color skinColor;
    Color hairColor;
    Color shoeColor;
    Color umbrellaColor;
    float seed;
    bool present;
    PersonState state;
    float waitTimer;
};

static std::vector<Person> people;
static const float S = 1.75f;

static bool firstUpdate  = true;
static bool prevDayState = false;

static float entryX(int dir)
{
    return dir > 0 ? -40.0f : 870.0f;
}

static float randRange(float a, float b)
{
    return a + (float)rand() / RAND_MAX * (b - a);
}

void initPeople()
{
    people.clear();

    struct Init { float y; float speed; int dir;
                  Color shirt, pant, skin, hair, shoe, umb; };

    Init inits[4] = {
        { 630, 22.0f, +1, Color(0.85f,0.25f,0.25f), Color(0.20f,0.25f,0.45f),
          Color(0.90f,0.70f,0.55f), Color(0.12f,0.08f,0.06f),
          Color(0.10f,0.10f,0.10f), Color(0.80f,0.20f,0.25f) },

        { 638, 18.0f, -1, Color(0.25f,0.55f,0.35f), Color(0.30f,0.20f,0.15f),
          Color(0.80f,0.60f,0.45f), Color(0.08f,0.06f,0.05f),
          Color(0.15f,0.10f,0.08f), Color(0.20f,0.35f,0.75f) },

        { 626, 25.0f, +1, Color(0.95f,0.75f,0.20f), Color(0.18f,0.18f,0.20f),
          Color(0.70f,0.50f,0.35f), Color(0.05f,0.05f,0.05f),
          Color(0.08f,0.08f,0.08f), Color(0.85f,0.65f,0.15f) },

        { 634, 20.0f, -1, Color(0.55f,0.30f,0.65f), Color(0.22f,0.22f,0.24f),
          Color(0.75f,0.55f,0.40f), Color(0.20f,0.14f,0.10f),
          Color(0.12f,0.12f,0.12f), Color(0.30f,0.70f,0.55f) },
    };

    for (int i = 0; i < 4; i++)
    {
        Person p;
        p.baseSpeed   = inits[i].speed;
        p.speedFactor = randRange(0.85f, 1.25f);
        p.dir   = inits[i].dir;
        p.y     = inits[i].y;
        p.x     = entryX(p.dir);
        p.walkPhase     = (float)i * 1.3f;
        p.shirtColor    = inits[i].shirt;
        p.pantColor     = inits[i].pant;
        p.skinColor     = inits[i].skin;
        p.hairColor     = inits[i].hair;
        p.shoeColor     = inits[i].shoe;
        p.umbrellaColor = inits[i].umb;
        p.seed = (float)i * 2.3f;
        p.present = false;
        p.state = WAITING;
        // Stagger initial appearance so they don't all pop in together
        p.waitTimer = randRange(1.0f, 8.0f) + i * 2.5f;
        people.push_back(p);
    }
}

// ============================================================
//  Re-stagger people waiting to walk, grouped by direction,
//  so two people from the SAME side never exit at the same time.
// ============================================================
static void restaggerByDirection()
{
    std::vector<int> group1, groupN1;
    for (size_t i = 0; i < people.size(); i++)
    {
        if (people[i].state != WAITING) continue;
        if (people[i].dir > 0) group1.push_back((int)i);
        else                   groupN1.push_back((int)i);
    }

    auto shuffleAndAssign = [](std::vector<int>& group)
    {
        // simple Fisher-Yates shuffle
        for (int i = (int)group.size() - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);
            std::swap(group[i], group[j]);
        }

        float t = randRange(0.5f, 3.0f);
        for (int idx : group)
        {
            people[idx].waitTimer = t;
            t += randRange(4.5f, 10.0f); // gap before next same-side person
        }
    };

    shuffleAndAssign(group1);
    shuffleAndAssign(groupN1);
}

void updatePeople(float dt)
{
    bool isDay = (dayTime > 0.2f && dayTime < 0.8f);
    float speedMul = isRaining ? 0.72f : 1.0f;

    if (firstUpdate)
    {
        prevDayState = isDay;
        firstUpdate  = false;
    }

    bool sunrise = (!prevDayState && isDay);
    prevDayState = isDay;

    if (sunrise)
        restaggerByDirection();

    for (auto& p : people)
    {
        if (p.state == WALKING)
        {
            float speed = p.baseSpeed * p.speedFactor * speedMul;
            p.x += p.dir * speed * dt;
            p.walkPhase += dt * speed * 0.30f;

            bool exitedRight = (p.dir > 0 && p.x > 900.0f);
            bool exitedLeft  = (p.dir < 0 && p.x < -70.0f);

            if (exitedRight || exitedLeft)
            {
                p.state = WAITING;
                p.present = false;
                p.waitTimer = randRange(3.0f, 12.0f);
            }
        }
        else // WAITING
        {
            p.present = false;

            if (isDay)
            {
                p.waitTimer -= dt;
                if (p.waitTimer <= 0.0f)
                {
                    p.state = WALKING;
                    p.present = true;
                    p.x = entryX(p.dir);
                    p.speedFactor = randRange(0.80f, 1.35f); // new speed each walk
                }
            }
            // At night: timer frozen, present stays false -> nobody spawns.
        }
    }
}

// ============================================================
//  UMBRELLA
// ============================================================
static void drawUmbrella(float hx, float hy, Color canopy, float sway)
{
    float r  = 13.5f * S;
    float cx = hx + sway;
    float cy = hy - 5.0f * S;

    setColor(0.22f, 0.22f, 0.25f, canopy.a);
    drawLine(hx, hy, cx, cy + 3.0f * S, 2.3f);

    int segs = 16;
    glBegin(GL_TRIANGLE_FAN);
        glColor4f(canopy.r, canopy.g, canopy.b, canopy.a);
        glVertex2f(cx, cy);
        for (int i = 0; i <= segs; i++)
        {
            float a = (float)M_PI + (float)i / segs * (float)M_PI;
            glVertex2f(cx + cosf(a) * r, cy + sinf(a) * r * 0.62f);
        }
    glEnd();

    setColor(canopy.r * 0.55f, canopy.g * 0.55f, canopy.b * 0.55f, canopy.a);
    glLineWidth(1.4f);
    glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= segs; i++)
        {
            float a = (float)M_PI + (float)i / segs * (float)M_PI;
            glVertex2f(cx + cosf(a) * r, cy + sinf(a) * r * 0.62f);
        }
    glEnd();

    setColor(canopy.r * 0.5f, canopy.g * 0.5f, canopy.b * 0.5f, canopy.a * 0.9f);
    for (int i = 1; i < segs; i += 3)
    {
        float a = (float)M_PI + (float)i / segs * (float)M_PI;
        drawLine(cx, cy, cx + cosf(a) * r, cy + sinf(a) * r * 0.62f, 1.0f);
    }

    setColor(1.0f, 1.0f, 1.0f, 0.22f * canopy.a);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx - r * 0.35f, cy - r * 0.08f);
        for (int i = 2; i <= 6; i++)
        {
            float a = (float)M_PI + (float)i / segs * (float)M_PI;
            glVertex2f(cx + cosf(a) * r * 0.88f, cy + sinf(a) * r * 0.52f);
        }
    glEnd();

    setColor(0.22f, 0.22f, 0.25f, canopy.a);
    drawCircle(cx, cy - 1.5f * S, 1.4f, 8);
}

// ============================================================
//  SINGLE PERSON
// ============================================================
static void drawSinglePerson(const Person& p)
{
    bool holdUmbrella = isRaining;
    int dir = p.dir;

    float phase  = p.walkPhase;
    float stride = holdUmbrella ? 0.65f : 1.0f;
    float swingA = sinf(phase) * stride;
    float swingB = sinf(phase + (float)M_PI) * stride;
    float bob    = fabsf(sinf(phase * 2.0f)) * 1.2f * S * (holdUmbrella ? 0.5f : 1.0f);

    float px = p.x;
    float groundY = p.y;
    float py = groundY - bob;

    float headR      = 5.6f * S;
    float bodyW      = 8.0f * S;
    float bodyH      = 14.5f * S;
    float neckH      = 1.6f * S;
    float thighLen   = 9.0f * S;
    float shinLen    = 8.2f * S;
    float upperArm   = 7.2f * S;
    float lowerArm   = 6.6f * S;

    float footY     = groundY;
    float kneeY     = footY - shinLen;
    float hipY      = kneeY - thighLen;
    float torsoTopY = hipY - bodyH;
    float neckY     = torsoTopY - neckH;
    float headY     = neckY - headR - 0.5f * S;
    float hipX      = px;

    setColor(0, 0, 0, 0.25f);
    drawCircle(px, groundY + 2, 8.5f * S * 0.5f, 14);

    float kneeBX = hipX + swingB * thighLen * 0.30f * dir;
    float footBX = kneeBX + swingB * shinLen * 0.80f * dir;
    float footBY = footY - fabsf(swingB) * 1.6f * S;

    setColor(p.pantColor.r*0.78f,p.pantColor.g*0.78f,p.pantColor.b*0.78f);
    drawLine(hipX, hipY, kneeBX, kneeY, 3.4f);
    drawLine(kneeBX, kneeY, footBX, footBY, 3.0f);
    setColor(p.pantColor.r*0.65f,p.pantColor.g*0.65f,p.pantColor.b*0.65f);
    drawCircle(kneeBX, kneeY, 1.7f * S * 0.5f, 8);
    setColor(p.shoeColor);
    drawCircle(footBX + 2.0f * dir, footBY, 2.0f * S, 10);

    float kneeAX = hipX + swingA * thighLen * 0.30f * dir;
    float footAX = kneeAX + swingA * shinLen * 0.80f * dir;
    float footAY = footY - fabsf(swingA) * 1.6f * S;

    setColor(p.pantColor);
    drawLine(hipX, hipY, kneeAX, kneeY, 3.6f);
    drawLine(kneeAX, kneeY, footAX, footAY, 3.2f);
    setColor(p.pantColor.r*0.85f,p.pantColor.g*0.85f,p.pantColor.b*0.85f);
    drawCircle(kneeAX, kneeY, 1.8f * S * 0.5f, 8);
    setColor(p.shoeColor);
    drawCircle(footAX + 2.0f * dir, footAY, 2.2f * S, 10);

    setColor(p.shirtColor);
    drawRect(px - bodyW / 2.0f, torsoTopY, bodyW, bodyH);
    setColor(fminf(p.shirtColor.r*1.18f,1.0f), fminf(p.shirtColor.g*1.18f,1.0f), fminf(p.shirtColor.b*1.18f,1.0f));
    drawRect(px - bodyW / 2.0f, torsoTopY, bodyW * 0.22f, bodyH);
    setColor(p.shirtColor.r*0.70f,p.shirtColor.g*0.70f,p.shirtColor.b*0.70f);
    drawRect(px + bodyW / 2.0f - bodyW * 0.22f, torsoTopY, bodyW * 0.22f, bodyH);
    setColor(p.shirtColor.r*0.55f,p.shirtColor.g*0.55f,p.shirtColor.b*0.55f);
    drawRect(px - bodyW * 0.18f, torsoTopY, bodyW * 0.36f, 2.0f * S);

    float shoulderBX = px - bodyW * 0.42f * dir;
    float shoulderBY = torsoTopY + 2.0f * S;
    float armSwingB  = -swingA;
    float elbowBX = shoulderBX + armSwingB * upperArm * 0.28f * dir;
    float elbowBY = shoulderBY + upperArm * 0.9f;
    float handBX  = elbowBX + armSwingB * lowerArm * 0.75f * dir;
    float handBY  = elbowBY + lowerArm * 0.9f;

    setColor(p.shirtColor.r*0.80f,p.shirtColor.g*0.80f,p.shirtColor.b*0.80f);
    drawLine(shoulderBX, shoulderBY, elbowBX, elbowBY, 2.6f);
    drawLine(elbowBX, elbowBY, handBX, handBY, 2.3f);
    setColor(p.skinColor);
    drawCircle(handBX, handBY, 1.7f * S * 0.5f, 8);

    float shoulderAX = px + bodyW * 0.42f * dir;
    float shoulderAY = torsoTopY + 2.0f * S;

    if (holdUmbrella)
    {
        float sway   = sinf(animTime * 1.4f + p.seed) * 2.2f;
        float elbowX = shoulderAX + 4.5f * S * dir;
        float elbowY = shoulderAY - 3.0f * S;
        float handX  = shoulderAX + 3.0f * S * dir + sway * 0.3f;
        float handY  = headY - 3.0f * S;

        setColor(p.shirtColor.r*0.92f,p.shirtColor.g*0.92f,p.shirtColor.b*0.92f);
        drawLine(shoulderAX, shoulderAY, elbowX, elbowY, 2.6f);
        drawLine(elbowX, elbowY, handX, handY, 2.3f);
        setColor(p.skinColor);
        drawCircle(handX, handY, 1.8f * S * 0.5f, 8);

        drawUmbrella(handX, handY, p.umbrellaColor, sway);
    }
    else
    {
        float armSwingA = -swingB;
        float elbowAX = shoulderAX + armSwingA * upperArm * 0.28f * dir;
        float elbowAY = shoulderAY + upperArm * 0.9f;
        float handAX  = elbowAX + armSwingA * lowerArm * 0.75f * dir;
        float handAY  = elbowAY + lowerArm * 0.9f;

        setColor(p.shirtColor);
        drawLine(shoulderAX, shoulderAY, elbowAX, elbowAY, 2.6f);
        drawLine(elbowAX, elbowAY, handAX, handAY, 2.3f);
        setColor(p.skinColor);
        drawCircle(handAX, handAY, 1.8f * S * 0.5f, 8);
    }

    setColor(p.skinColor);
    drawRect(px - 1.6f * S * 0.5f, neckY, 3.2f * S * 0.5f, neckH);

    setColor(p.skinColor);
    drawCircle(px, headY, headR, 18);

    setColor(p.hairColor);
    drawCircle(px, headY - headR * 0.35f, headR * 1.02f, 18);
    setColor(p.skinColor);
    drawCircle(px, headY + headR * 0.12f, headR * 0.92f, 18);
    setColor(p.hairColor);
    drawRect(px - headR * 0.95f, headY - headR * 0.85f, headR * 1.9f, headR * 0.55f);

    setColor(0.08f, 0.08f, 0.08f);
    drawCircle(px + dir * headR * 0.42f, headY - headR * 0.05f, 0.9f * S * 0.5f, 6);

    setColor(p.skinColor.r*0.75f,p.skinColor.g*0.55f,p.skinColor.b*0.55f);
    drawLine(px + dir * headR * 0.15f, headY + headR * 0.42f,
             px + dir * headR * 0.55f, headY + headR * 0.42f, 1.0f);

    if (holdUmbrella)
    {
        for (int i = 0; i < 3; i++)
        {
            float t = fmodf(animTime * 1.5f + i * 0.3f + p.seed, 1.0f);
            float dx = (p.x - 60.0f * t) + (float)dir * 2.0f;
            float dy = footAY + 4.0f + t * 10.0f;
            setColor(0.6f, 0.75f, 0.9f, 0.35f * (1.0f - t));
            drawCircle(dx, dy, 1.2f, 6);
        }
    }
}

void drawPeople()
{
    for (auto& p : people)
    {
        if (!p.present) continue;
        drawSinglePerson(p);
    }
}