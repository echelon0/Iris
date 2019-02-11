
struct film {
    void *Buffer;
    void *SumBuffer;    
    int PixelWidth;
    int PixelHeight;
    vec2 WorldSize;
    f32 Dist;
};

struct camera {
    vec3 Pos;
    vec3 Dir;
    vec3 Up;
    vec3 Right;
    quat Rot;

    film Film;
    u32 SampleCount;
    bool Updated;
};

void
InitCamera(camera *Camera, u32 WindowWidth, u32 WindowHeight) {
    Camera->Pos = vec3(0.0f, 0.0f, 0.0f);
    Camera->Dir = vec3(0.0f, 0.0f, 1.0f);
    Camera->Up = vec3(0.0f, 1.0f, 0.0f);
    Camera->Right = vec3(1.0f, 0.0f, 0.0f);
    Camera->Rot = quat_from_euler_angles(vec3((f32)PI, 0.0f, 0.0f));
            
    Camera->Film.PixelWidth = WindowWidth;
    Camera->Film.PixelHeight = WindowHeight;
    Camera->Film.Buffer = (void *)malloc(Camera->Film.PixelWidth * Camera->Film.PixelHeight * sizeof(u32));
    Camera->Film.SumBuffer = (void *)calloc(Camera->Film.PixelWidth * Camera->Film.PixelHeight * sizeof(u32) * 4, 1);
    f32 aspect = (f32)WindowWidth / (f32)WindowHeight;
    Camera->Film.WorldSize = vec2(aspect, 1.0f);
    Camera->Film.Dist = 1.0f;
}

void
UpdateCameraScene(camera *Camera, scene *Scene) {
    if(Camera->Updated || Scene->Updated) {
        memset(Camera->Film.SumBuffer, (u8)0, Camera->Film.PixelWidth * Camera->Film.PixelHeight * sizeof(u32) * 4);
        Camera->SampleCount = 0;
        Camera->Updated = false;
        Scene->Updated = false;
    }
}

void
ProcessInput(camera *Camera) {
    //Movement
    f32 speed = 0.5f;
    vec3 velocity = vec3(0.0f, 0.0f, 0.0f);
    if(GlobalInputState.W_KEY) {
        velocity += (Camera->Dir * speed);
    }
    if(GlobalInputState.A_KEY) {
        velocity += (-Camera->Right * speed);
    }
    if(GlobalInputState.S_KEY) {
        velocity += (-Camera->Dir * speed);
    }
    if(GlobalInputState.D_KEY) {
        velocity += (Camera->Right * speed);  
    }
    if(GlobalInputState.SPACE_KEY) {
        velocity += (vec3(0.0f, 1.0f, 0.0f) * speed);
    }
    if(GlobalInputState.CTRL_KEY) {
        velocity -= (vec3(0.0f, 1.0f, 0.0f) * speed);
    }
    f32 disp = magnitude(velocity);
    if(disp > 0.0001f) {
        Camera->Updated = true;
        if(disp > 1.0f) 
            velocity = velocity * (disp / (f32)sqrt(2.0f));
        Camera->Pos += velocity;
    }

    //Rotation
    if(GlobalInputState.LEFT_MOUSE_DOWN) {
        GlobalInputState.PER_FRAME_DRAG_VECTOR = GlobalInputState.CURRENT_POS - ivec2(472, 259);
        if(GlobalInputState.PER_FRAME_DRAG_VECTOR != ivec2(0, 0)) {
            GlobalInputState.PER_FRAME_DRAG_VECTOR_PERCENT = vec2((f32)GlobalInputState.PER_FRAME_DRAG_VECTOR.x / (f32)GetSystemMetrics(SM_CXSCREEN),
                                                                  (f32)GlobalInputState.PER_FRAME_DRAG_VECTOR.y / (f32)GetSystemMetrics(SM_CYSCREEN));

            f32 ThetaX = GlobalInputState.PER_FRAME_DRAG_VECTOR_PERCENT.x;
            f32 ThetaY = GlobalInputState.PER_FRAME_DRAG_VECTOR_PERCENT.y;  
            rotate(&Camera->Dir, ThetaX, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
            rotate(&Camera->Right, ThetaX, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
            rotate(&Camera->Up, ThetaX, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));    
            rotate(&Camera->Up, ThetaY, vec3(0.0f, 0.0f, 0.0f), Camera->Right);
            rotate(&Camera->Dir, ThetaY, vec3(0.0f, 0.0f, 0.0f), Camera->Right);    
            Camera->Updated = true;
        }
    }
}
