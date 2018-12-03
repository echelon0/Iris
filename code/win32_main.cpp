
#define LOG_ERROR(Title, Message) MessageBoxA(0, Message, Title, MB_OK|MB_ICONERROR)
#define LOOP(LOOP_COUNT) for(int COUNTER = 0; COUNTER < LOOP_COUNT; COUNTER++);
#define u8  unsigned char
#define u32 unsigned int
#define f32 float

#include <windows.h>
#include <stdio.h>
#include <float.h>
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
            WORD windowWidth = LOWORD(lParam);
            WORD windowHeight = HIWORD(lParam);           
        } break;

        default: {
            result = DefWindowProc(hWnd, Msg, wParam, lParam);
        } break;
    }
    
    return result;
}

ivec2 CalcWindowPos(int windowWidth, int windowHeight) {
    ivec2 pos;
    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    pos.y = (screen_height - windowHeight) / 2;
    pos.x = (screen_width - windowWidth) / 2;
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
    int windowWidth = 960;
    int windowHeight = 580;
    float aspect = (f32)windowWidth / (f32)windowHeight;
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

            //----------------- testing --------------------
            Camera camera = {};
            camera.pos = vec3(0.0f, 0.0f, 0.0f);
            camera.dir = vec3(0.0f, 0.0f, 1.0f);
            camera.up = vec3(0.0f, 1.0f, 0.0f);
            camera.right = vec3(1.0f, 0.0f, 0.0f);
            
            camera.film.pixelWidth = windowWidth;
            camera.film.pixelHeight = windowHeight;
            camera.film.buffer = (void *)malloc(camera.film.pixelWidth * camera.film.pixelHeight * sizeof(u32));
            camera.film.worldSize = vec2(aspect, 1.0f);
            camera.film.dist = 1.0f;
            
            Scene scene = {};
            Entity entity = {};
            entity.isShape = 1;
            entity.shape.material.diffuse = vec3(0.0f, 0.0f, 1.0f);         
            entity.shape.radius = 2.0f;
            entity.pos = vec3(2.5f, 0.0f, 10.0f);
            scene.entities.PushBack(entity);

            entity.isShape = 1;
            entity.shape.material.diffuse = vec3(1.0f, 0.0f, 0.0f);         
            entity.shape.radius = 2.0f;
            entity.pos = vec3(-2.5f, 0.0f, 10.0f);
            scene.entities.PushBack(entity);        
            
            //----------------------------------------------
            
            while(g_IsRunning) {
                camera.pos.z = camera.pos.z + (f32)sin(g_iTime);
                LARGE_INTEGER beginCount;
                QueryPerformanceCounter(&beginCount);

                if(!Sample(camera, scene)) {
                    g_IsRunning = false;
                    continue;
                }
                
                //TODO: pad scan lines on a LONG data type boudary
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
                if(frameDuration < FRAME_FREQUENCY) {
                    Sleep((DWORD)(FRAME_FREQUENCY - frameDuration));
                }
                g_iTime++;
            }            
        }
    }
}


