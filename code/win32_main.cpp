
#define LOG_ERROR(Title, Message) MessageBoxA(0, Message, Title, MB_OK|MB_ICONERROR)
#define u8  unsigned char
#define u32 unsigned int
#define f32 float

#include <windows.h>
#include <stdio.h>
#include "string.cpp"
#include "array.cpp"
#include "math.h"
#include "renderer.cpp"
#include "file_loader.cpp"

static bool g_IsRunning = false;
static int g_iTime = 0;

LRESULT CALLBACK CallWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    switch(Msg) {
        case WM_CLOSE: {
            g_IsRunning = false;
        } break;
            
        case WM_SIZE: {
            WORD window_width = LOWORD(lParam);
            WORD window_height = HIWORD(lParam);           
        } break;

        default: {
            result = DefWindowProc(hWnd, Msg, wParam, lParam);
        } break;
    }
    
    return result;
}

ivec2 CalcWindowPos(int window_width, int window_height) {
    ivec2 pos;
    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    pos.y = (screen_height - window_height) / 2;
    pos.x = (screen_width - window_width) / 2;
    return pos;
}

f32 GetElapsedTime(LARGE_INTEGER beginCount) {
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

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    int window_width = 960;
    int window_height = 580;
    ivec2 window_pos = CalcWindowPos(window_width, window_height);
    
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
            window_width, window_height,
            0, 0,
            hInstance, 0);
        
        if(window) {
            f32 FRAME_RATE = 60.0f;
            f32 FRAME_FREQUENCY = (1000.0f / FRAME_RATE);
            g_IsRunning = true;

            while(g_IsRunning) {
                LARGE_INTEGER beginCount;
                QueryPerformanceCounter(&beginCount);
                                    
                MSG message;
                while(PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
                
                f32 frameDuration = GetElapsedTime(beginCount);
                if(frameDuration < FRAME_FREQUENCY) {
                    Sleep((DWORD)(FRAME_FREQUENCY - frameDuration));
                }
                g_iTime++;
            }            
        }
    }
}


