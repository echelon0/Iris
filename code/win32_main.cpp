
#define LOG_ERROR(Title, Message) MessageBoxA(0, Message, Title, MB_OK|MB_ICONERROR)

#define u8  unsigned char
#define u32 unsigned int
#define f32 float

#include <Windows.h>
#include <stdio.h>
#include "iris.h"
#include <thread>

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
            WORD WindowWidth = LOWORD(lParam);
            WORD WindowHeight = HIWORD(lParam);
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
CalcWindowPos(int WindowWidth, int WindowHeight) {
    ivec2 pos;
    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    pos.y = (screen_height - WindowHeight) / 2;
    pos.x = (screen_width - WindowWidth) / 2;
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
ProcessInput(camera *Camera) {

    //camera control
    f32 speed = 0.5f;
    vec3 velocity = vec3(0.0f, 0.0f, 0.0f);
    if(g_inputState.W_KEY) {
        velocity += (Camera->Dir * speed);
    }
    if(g_inputState.A_KEY) {
        velocity += (-Camera->Right * speed);
    }
    if(g_inputState.S_KEY) {
        velocity += (-Camera->Dir * speed);
    }
    if(g_inputState.D_KEY) {
        velocity += (Camera->Right * speed);  
    }
    f32 disp = magnitude(velocity);
    if(disp > 0.0001f) {
        Camera->Updated = true;
        if(disp > 1.0f) 
            velocity = velocity * (disp / (f32)sqrt(2.0f));
        Camera->Pos += velocity;
    }
}

int CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    int WindowWidth = 960;
    int WindowHeight = 580;
    f32 Aspect = (f32)WindowWidth / (f32)WindowHeight;
    ivec2 WindowPos = CalcWindowPos(WindowWidth, WindowHeight);
    
    WNDCLASS WindowClass = {};
    WindowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
    WindowClass.lpfnWndProc = CallWindowProc;
    WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
    WindowClass.hInstance = hInstance;
    WindowClass.lpszClassName = "Iris";   

    if(RegisterClass(&WindowClass)) {
        HWND Window = CreateWindow(
            WindowClass.lpszClassName,
            "Iris",
            WS_VISIBLE|WS_OVERLAPPEDWINDOW,
            WindowPos.x, WindowPos.y,
            WindowWidth, WindowHeight,
            0, 0,
            hInstance, 0);

        if(Window) {
            f32 FRAME_RATE = 60.0f;
            f32 FRAME_FREQUENCY = (1000.0f / FRAME_RATE);
            g_IsRunning = true;

            //----------------- Iris --------------------
            camera Camera = {};
            scene Scene = {};
            InitCamera(&Camera, WindowWidth, WindowHeight);
            DemoScene2(&Camera, &Scene);
            EnableMultithreading();
            //----------------------------------------------

            while(g_IsRunning) {
                LARGE_INTEGER BeginCount;
                QueryPerformanceCounter(&BeginCount);

                //------------------- Iris -------------------
                ProcessInput(&Camera);
                u32 PathDepth = 8;
                u32 nDirectSamples = 1;
                if(Camera.SampleCount < 1000) {
                    Draw(&Camera, &Scene, PathDepth, nDirectSamples);
                }
                //--------------------------------------------
                
                HDC DeviceContext = GetWindowDC(Window);
                RenderBuffer(DeviceContext, Camera.Film.Buffer,
                             WindowWidth, WindowHeight,
                             Camera.Film.PixelWidth, Camera.Film.PixelHeight);
                ReleaseDC(Window, DeviceContext);
                
                
                MSG message;
                while(PeekMessage(&message, Window, 0, 0, PM_REMOVE)) {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
                
                f32 FrameDuration = GetElapsedTime(BeginCount);
                f32 WaitTime = FRAME_FREQUENCY - FrameDuration;
                if(WaitTime > 0.0f) {
                    Sleep((DWORD)WaitTime);
                }

                char WindowText[256];
                sprintf(WindowText, "Iris -- fps:%.2f, Samples: %d -- Thread count: %d", 1000.0f / (FrameDuration + ((WaitTime > 0)? WaitTime : 0)), Camera.SampleCount, Multithreading.MaxThreads);
                SetWindowTextA(Window, WindowText);
                g_iTime++;
            }
        }
    }
}


