
#define LOG_ERROR(Title, Message) MessageBoxA(0, Message, Title, MB_OK|MB_ICONERROR)

#define u8  unsigned char
#define u32 unsigned int
#define f32 float

#include <windows.h>
#include <stdio.h>
#include "iris.h"

static bool g_IsRunning = false;
static u32 g_iTime = 0;

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
            entity.isShape = true;
            entity.shape.type = SPHERE;
            entity.shape.material.diffuse = vec3(0.38f, 0.67f, 0.90f);         
            entity.shape.radius = 2.0f;
            entity.pos = vec3(2.5f, 0.0f, 9.0f);
            scene.entities.PushBack(entity);

            entity.isShape = true;
            entity.shape.type = SPHERE;
            entity.shape.material.diffuse = vec3(0.97f, 0.3f, 0.2f);
            entity.shape.radius = 2.0f;
            entity.pos = vec3(-2.5f, 0.0f, 9.0f);
            scene.entities.PushBack(entity);

            entity.isShape = true;
            entity.shape.type = PLANE;
            entity.shape.material.diffuse = vec3(0.8f, 0.87f, 0.99f);
            entity.shape.pPlane = vec3(0.0, -2.0f, 0.0);
            entity.shape.nPlane = normalize(vec3(0.0f, 1.0f, 0.0f));
            scene.entities.PushBack(entity);

            //----------------------------------------------
            InitIris();
            while(g_IsRunning) {
                scene.entities[0].pos += vec3((f32)sin(g_iTime)*(-0.128f), (f32)sin(g_iTime)*0.854f, (f32)sin(g_iTime)*(-1.2f));
                scene.entities[1].pos += vec3((f32)sin(g_iTime)*(0.123f), (f32)sin(g_iTime)*(-0.93f), (f32)sin(g_iTime)*1.7f);
                LARGE_INTEGER beginCount;
                QueryPerformanceCounter(&beginCount);

                if(!Draw(camera, scene, 32)) {
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
                f32 waitTime = FRAME_FREQUENCY - frameDuration;
                if(waitTime > 0.0f) {
                    Sleep((DWORD)waitTime);
                }

                char windowText[256];
                sprintf(windowText, "Iris -- fps:%.2f", 1000.0f / (frameDuration + ((waitTime > 0)? waitTime : 0)));
                SetWindowTextA(window, windowText);
                g_iTime++;
            }
            FreeIris();
        }
    }
}


