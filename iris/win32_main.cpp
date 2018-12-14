
#define LOG_ERROR(Title, Message) MessageBoxA(0, Message, Title, MB_OK|MB_ICONERROR)

#define u8  unsigned char
#define u32 unsigned int
#define f32 float

#include <windows.h>
#include <stdio.h>
#include "iris.h"

static bool g_IsRunning = false;
static u32 g_iTime = 0;

struct input_state {
    bool W_KEY;
    bool A_KEY;
    bool S_KEY;
    bool D_KEY;
};

input_state g_inputState = {};

LRESULT CALLBACK CallWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    switch(Msg) {
        case WM_CLOSE: {
            g_IsRunning = false;
        } break;

        case WM_SIZE: {
            WORD windowWidth = LOWORD(lParam);
            WORD windowHeight = HIWORD(lParam);
        } break;

        case WM_KEYDOWN: {
            if(wParam == 'W') { //W_KEY
                g_inputState.W_KEY = true;
            }
            if(wParam == 'A') { //A_KEY
                g_inputState.A_KEY = true;
            }
            if(wParam == 'S') { //S_KEY
                g_inputState.S_KEY = true;
            }
            if(wParam == 'D') { //D_KEY
                g_inputState.D_KEY = true;
            }       
        } break;

        case WM_KEYUP: {
            if(wParam == 'W') { //W_KEY
                g_inputState.W_KEY = false;
            }
            if(wParam == 'A') { //A_KEY
                g_inputState.A_KEY = false;
            }
            if(wParam == 'S') { //S_KEY
                g_inputState.S_KEY = false;
            }
            if(wParam == 'D') { //D_KEY
                g_inputState.D_KEY = false;
            }       
        } break;

        default: {
            result = DefWindowProc(hWnd, Msg, wParam, lParam);
        } break;
    }
    
    return result;
}

ivec2
CalcWindowPos(int windowWidth, int windowHeight) {
    ivec2 pos;
    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    pos.y = (screen_height - windowHeight) / 2;
    pos.x = (screen_width - windowWidth) / 2;
    return pos;
}

f32
GetElapsedTime(LARGE_INTEGER beginCount) {
    timeBeginPeriod(1);
    LARGE_INTEGER frequency;
    if(QueryPerformanceFrequency(&frequency)) {
        LARGE_INTEGER Count;
        QueryPerformanceCounter(&Count);
        timeEndPeriod(1);
        return ((f32)(Count.QuadPart - beginCount.QuadPart) / (f32)frequency.QuadPart) * 1000.0f; // time in miliseconds
    } else {
        return (f32)GetTickCount();
    }
}

void
ProcessInput(Camera *camera) {

    //camera control
    f32 speed = 0.5f;
    vec3 velocity = vec3(0.0f, 0.0f, 0.0f);
    if(g_inputState.W_KEY) {
        velocity += (camera->dir * speed);
    }
    if(g_inputState.A_KEY) {
        velocity += (-camera->right * speed);
    }
    if(g_inputState.S_KEY) {
        velocity += (-camera->dir * speed);
    }
    if(g_inputState.D_KEY) {
        velocity += (camera->right * speed);  
    }
    f32 disp = magnitude(velocity);
    if(disp > 0.0001f) {
        camera->updated = true;
        if(disp > 1.0f) 
            velocity = velocity * (disp / (f32)sqrt(2.0f));
        camera->pos += velocity;
    }
}

int CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    int windowWidth = 960;
    int windowHeight = 580;
    f32 aspect = (f32)windowWidth / (f32)windowHeight;
    ivec2 window_pos = CalcWindowPos(windowWidth, windowHeight);
    
    WNDCLASS window_class = {};
    window_class.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
    window_class.lpfnWndProc = CallWindowProc;
    window_class.hCursor = LoadCursor(0, IDC_ARROW);
    window_class.hInstance = hInstance;
    window_class.lpszClassName = "Iris";   

    if(RegisterClass(&window_class)) {
        HWND window = CreateWindow(
            window_class.lpszClassName,
            "Iris",
            WS_VISIBLE|WS_OVERLAPPEDWINDOW,
            window_pos.x, window_pos.y,
            windowWidth, windowHeight,
            0, 0,
            hInstance, 0);

        if(window) {
            f32 FRAME_RATE = 60.0f;
            f32 FRAME_FREQUENCY = (1000.0f / FRAME_RATE);
            g_IsRunning = true;

            //----------------- Iris --------------------
            Camera camera = {};
            Scene scene = {};
            InitCamera(&camera, windowWidth, windowHeight);
            DemoScene1(&camera, &scene);
            //----------------------------------------------

            while(g_IsRunning) {
                LARGE_INTEGER beginCount;
                QueryPerformanceCounter(&beginCount);

                //------------------- Iris -------------------
                ProcessInput(&camera);
                if(camera.sampleCount < 10000)
                    Draw(&camera, &scene);
//                DrawMultiThread(&camera, &scene);
                //--------------------------------------------
                
                HDC deviceContext = GetWindowDC(window);
                RenderBuffer(deviceContext, camera.film.buffer,
                             windowWidth, windowHeight,
                             camera.film.pixelWidth, camera.film.pixelHeight);
                ReleaseDC(window, deviceContext);
                
                
                MSG message;
                while(PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
                
                f32 frameDuration = GetElapsedTime(beginCount);
                f32 waitTime = FRAME_FREQUENCY - frameDuration;
                if(waitTime > 0.0f) {
                    Sleep((DWORD)waitTime);
                }

                char windowText[256];
                sprintf(windowText, "Iris -- fps:%.2f, samples: %d", 1000.0f / (frameDuration + ((waitTime > 0)? waitTime : 0)), camera.sampleCount);
                SetWindowTextA(window, windowText);
                g_iTime++;
            }
        }
    }
}


