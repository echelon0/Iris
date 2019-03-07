
#define LOG_ERROR(Title, Message) MessageBoxA(0, Message, Title, MB_OK|MB_ICONERROR)

#define u8  unsigned char
#define u32 unsigned int
#define f32 float

#include <Windows.h>
#include <Windowsx.h>
#include <stdio.h>
#include <thread>
#include "math.h"


static bool g_IsRunning = false;
static ivec2 WindowSize = ivec2(0, 0);
static ivec2 ClientCenter = ivec2(0, 0);
static u32 g_iTime = 0;

struct input_state {
    bool W_KEY;
    bool A_KEY;
    bool S_KEY;
    bool D_KEY;
    bool SPACE_KEY;
    bool CTRL_KEY;

    bool LEFT_MOUSE_DOWN;
    ivec2 PREV_POS;
    ivec2 CURRENT_POS;
    bool SET_DRAG_VECTOR;
    ivec2 PER_FRAME_DRAG_VECTOR;
    vec2 PER_FRAME_DRAG_VECTOR_PERCENT;
};

input_state GlobalInputState = {};

#include "iris.h"

LRESULT CALLBACK CallWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    if(GetActiveWindow() != hWnd) {
        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
    
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
            if(wParam == VK_ESCAPE) {
                g_IsRunning = false;
            }
            if(wParam == 'W') { //W_KEY
                GlobalInputState.W_KEY = true;
            }
            if(wParam == 'A') { //A_KEY
                GlobalInputState.A_KEY = true;
            }
            if(wParam == 'S') { //S_KEY
                GlobalInputState.S_KEY = true;
            }
            if(wParam == 'D') { //D_KEY
                GlobalInputState.D_KEY = true;
            }
            if(wParam == VK_SPACE) {
                GlobalInputState.SPACE_KEY = true;              
            }
            if(wParam == VK_CONTROL) {
                GlobalInputState.CTRL_KEY = true;              
            }                    
        } break;

        case WM_KEYUP: {       
            if(wParam == 'W') { //W_KEY
                GlobalInputState.W_KEY = false;
            }
            if(wParam == 'A') { //A_KEY
                GlobalInputState.A_KEY = false;
            }
            if(wParam == 'S') { //S_KEY
                GlobalInputState.S_KEY = false;
            }
            if(wParam == 'D') { //D_KEY
                GlobalInputState.D_KEY = false;
            }
            if(wParam == VK_SPACE) {
                GlobalInputState.SPACE_KEY = false;              
            }
            if(wParam == VK_CONTROL) {
                GlobalInputState.CTRL_KEY = false;              
            }       
        } break;

        case WM_LBUTTONDOWN: {
            GlobalInputState.LEFT_MOUSE_DOWN = true;
        } break;

        case WM_LBUTTONUP: {
            GlobalInputState.LEFT_MOUSE_DOWN = false;
        } break;            

        case WM_MOUSEMOVE: {
            GlobalInputState.PREV_POS = GlobalInputState.CURRENT_POS;
            GlobalInputState.CURRENT_POS.x = GET_X_LPARAM(lParam);
            GlobalInputState.CURRENT_POS.y = GET_Y_LPARAM(lParam);
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

int CALLBACK
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    int WindowWidth = 960;
    int WindowHeight = 580;
    WindowSize = ivec2(WindowWidth, WindowHeight);
    f32 Aspect = (f32)WindowWidth / (f32)WindowHeight;
    ivec2 WindowPos = CalcWindowPos(WindowWidth, WindowHeight);
    ClientCenter = ivec2(WindowPos.x + WindowSize.x / 2, WindowPos.y + WindowSize.y / 2);

    
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
            DemoScene5(&Camera, &Scene);
            EnableMultithreading();
            //----------------------------------------------

            while(g_IsRunning) {
                if(GetActiveWindow() == Window) {
                    SetCursorPos(ClientCenter.x, ClientCenter.y);
                    ShowCursor(0);
                }
                LARGE_INTEGER BeginCount;
                QueryPerformanceCounter(&BeginCount);

                //------------------- Iris -------------------
                ProcessInput(&Camera);
                UpdateCameraScene(&Camera, &Scene);
                u32 PathDepth = 2;
                u32 nDirectSamples = 1;
                if(Camera.SampleCount < 512) {
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


