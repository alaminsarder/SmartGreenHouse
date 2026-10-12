#include "window.h"

// Forward declarations from main.cpp
extern void display();
extern void update(float dt);
extern void onKeyDown(int key);
extern void onSpecialKey(int key);

// Windows-specific handles
static HDC   g_hDC   = NULL;
static HGLRC g_hRC   = NULL;
static HWND  g_hWnd  = NULL;
static HINSTANCE g_hInstance = NULL;
static bool  g_active = true;
static bool  g_running = true;

// ============================================================
//  WINDOW PROCEDURE (handles Windows messages)
// ============================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_ACTIVATE:
        g_active = (LOWORD(wParam) != WA_INACTIVE);
        return 0;

    case WM_SYSCOMMAND:
        // Prevent screensaver, monitor power off
        if (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER)
            return 0;
        break;

    case WM_CLOSE:
        g_running = false;
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN:
        // ESC to exit
        if (wParam == VK_ESCAPE) {
            g_running = false;
            PostQuitMessage(0);
            return 0;
        }
        
        // Special keys (arrows)
        if (wParam == VK_UP || wParam == VK_DOWN ||
            wParam == VK_LEFT || wParam == VK_RIGHT) {
            onSpecialKey((int)wParam);
        } else {
            onKeyDown((int)wParam);
        }
        return 0;

    case WM_SIZE:
        {
            int w = LOWORD(lParam);
            int h = HIWORD(lParam);
            glViewport(0, 0, w, h);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(0, WIN_W, WIN_H, 0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
        }
        return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// ============================================================
//  CREATE OPENGL WINDOW
// ============================================================
bool createGLWindow(const char* title, int width, int height)
{
    WNDCLASS wc = {};
    g_hInstance = GetModuleHandle(NULL);

    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = g_hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "SmartGreenhouseClass";

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "Failed to register window class!", "Error", MB_OK);
        return false;
    }

    // Adjust window size to include title bar
    RECT winRect = { 0, 0, width, height };
    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    AdjustWindowRect(&winRect, style, FALSE);

    g_hWnd = CreateWindow(
        "SmartGreenhouseClass",
        title,
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        winRect.right - winRect.left,
        winRect.bottom - winRect.top,
        NULL, NULL, g_hInstance, NULL
    );

    if (!g_hWnd) {
        MessageBox(NULL, "Failed to create window!", "Error", MB_OK);
        return false;
    }

    // Get Device Context
    g_hDC = GetDC(g_hWnd);
    if (!g_hDC) {
        MessageBox(NULL, "Failed to get DC!", "Error", MB_OK);
        return false;
    }

    // Pixel Format
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cColorBits   = 32;
    pfd.cDepthBits   = 24;
    pfd.iLayerType   = PFD_MAIN_PLANE;

    int pf = ChoosePixelFormat(g_hDC, &pfd);
    if (!pf) {
        MessageBox(NULL, "Cannot find pixel format!", "Error", MB_OK);
        return false;
    }

    if (!SetPixelFormat(g_hDC, pf, &pfd)) {
        MessageBox(NULL, "Cannot set pixel format!", "Error", MB_OK);
        return false;
    }

    // OpenGL Rendering Context
    g_hRC = wglCreateContext(g_hDC);
    if (!g_hRC) {
        MessageBox(NULL, "Cannot create OpenGL context!", "Error", MB_OK);
        return false;
    }

    if (!wglMakeCurrent(g_hDC, g_hRC)) {
        MessageBox(NULL, "Cannot activate OpenGL context!", "Error", MB_OK);
        return false;
    }

    // Show window
    ShowWindow(g_hWnd, SW_SHOW);
    SetForegroundWindow(g_hWnd);
    SetFocus(g_hWnd);

    // Setup OpenGL
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIN_W, WIN_H, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0, 0, 0, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Build font for text rendering
    buildFont();

    return true;
}

// ============================================================
//  BUILD FONT (Windows Bitmap Font)
// ============================================================
void buildFont()
{
    HFONT font = CreateFont(
        -14,                        // Height
        0,                          // Width
        0, 0,                       // Escapement, Orientation
        FW_NORMAL,                  // Weight
        FALSE, FALSE, FALSE,        // Italic, Underline, Strikeout
        ANSI_CHARSET,
        OUT_TT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY,
        FF_DONTCARE | DEFAULT_PITCH,
        "Arial"
    );

    fontBase = glGenLists(96);
    HFONT oldFont = (HFONT)SelectObject(g_hDC, font);
    wglUseFontBitmaps(g_hDC, 32, 96, fontBase);
    SelectObject(g_hDC, oldFont);
    DeleteObject(font);
}

// ============================================================
//  KILL FONT
// ============================================================
void killFont()
{
    if (fontBase) {
        glDeleteLists(fontBase, 96);
        fontBase = 0;
    }
}

// ============================================================
//  DESTROY WINDOW
// ============================================================
void destroyGLWindow()
{
    killFont();

    if (g_hRC) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(g_hRC);
        g_hRC = NULL;
    }

    if (g_hDC) {
        ReleaseDC(g_hWnd, g_hDC);
        g_hDC = NULL;
    }

    if (g_hWnd) {
        DestroyWindow(g_hWnd);
        g_hWnd = NULL;
    }

    UnregisterClass("SmartGreenhouseClass", g_hInstance);
}

// ============================================================
//  MAIN LOOP
// ============================================================
void runMainLoop()
{
    MSG msg;
    DWORD lastTime = GetTickCount();

    while (g_running)
    {
        // Process all pending messages
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) {
                g_running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!g_running) break;

        // Calculate delta time
        DWORD currentTime = GetTickCount();
        deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        // Cap deltaTime to prevent huge jumps
        if (deltaTime > 0.1f) deltaTime = 0.1f;

        animTime += deltaTime;

        // Update game logic
        update(deltaTime);

        // Render
        display();

        // Swap buffers
        SwapBuffers(g_hDC);

        // Frame limiter (~60 FPS)
        Sleep(16);
    }
}

HWND getWindowHandle()
{
    return g_hWnd;
}