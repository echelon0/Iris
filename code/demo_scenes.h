
void
DemoScene1(camera *Camera, scene *Scene) {
    Camera->Pos = vec3(0.0f, 0.0f, 0.0f);
    Camera->Dir = vec3(0.0f, 0.0f, 1.0f);
    Camera->Up = vec3(0.0f, 1.0f, 0.0f);
    Camera->Right = vec3(1.0f, 0.0f, 0.0f);
    Camera->Film.Dist = 1.0f;

    entity Entity = {};

    vec3 Start = vec3(-3.0f, -0.5f, 6.0f);
    Entity.IsShape = true;
    Entity.Shape.Type = SPHERE;
    Entity.Shape.Material.Diffuse = rgb(255, 0, 0);
    Entity.Shape.Radius = 1.0f;
    Entity.Offset = Start;
    Scene->Entities.PushBack(Entity);

    vec3 spacing = vec3((2.0f * absf(Start.x) / 3.0f), 0.0f, 0.0f);

    Entity.Shape.Material.Diffuse = rgb(0, 255, 0);
    Entity.Offset += spacing;
    Scene->Entities.PushBack(Entity);

    Entity.Shape.Material.Diffuse = rgb(0, 0, 255);
    Entity.Offset += spacing;
    Scene->Entities.PushBack(Entity);

    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Offset += spacing;
    Scene->Entities.PushBack(Entity);

    //ground
    Entity.IsShape = true;
    Entity.Shape.Type = PLANE;
    Entity.Shape.Material.Diffuse = rgb(180, 180, 180);
    Entity.Shape.pPlane = vec3(0.0, -1.5f, 0.0);
    Entity.Shape.nPlane = vec3(0.0f, 1.0f, 0.0f);
    Scene->Entities.PushBack(Entity);

    //left wall
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.pPlane = vec3(-4.0, 0.0f, 0.0);
    Entity.Shape.nPlane = vec3(1.0f, 0.0f, 0.0f);
    Scene->Entities.PushBack(Entity);

    //Right wall
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.pPlane = vec3(4.0, 0.0f, 0.0);
    Entity.Shape.nPlane = vec3(-1.0f, 0.0f, 0.0f);
    Scene->Entities.PushBack(Entity);

    //front wall
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.pPlane = vec3(0.0, 0.0f, 8.0);
    Entity.Shape.nPlane = vec3(0.0f, 0.0f, -1.0f);
    Scene->Entities.PushBack(Entity);

    //back wall
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.pPlane = vec3(0.0, 0.0f, -2.0);
    Entity.Shape.nPlane = vec3(0.0f, 0.0f, 1.0f);
    Scene->Entities.PushBack(Entity);

    //Light
    entity Light = {};
    Light.IsShape = true;
    Light.Shape.Type = SPHERE;    
    Light.IsEmitter = true;    
    Light.Shape.Radius = 3.0f;
    Light.Emission.Flux = 100.0f;
    Light.Emission.Color = rgb(255, 255, 255);
    Light.Offset = vec3(0.0f, 5.0f, 2.0f);    
    Scene->Entities.PushBack(Light);
}

void 
DemoScene2(camera *Camera, scene *Scene) {
    Camera->Pos = vec3(0.0f, 0.0f, -1.5f);
    Camera->Dir = vec3(0.0f, 0.0f, 1.0f);
    Camera->Up = vec3(0.0f, 1.0f, 0.0f);
    Camera->Right = vec3(1.0f, 0.0f, 0.0f);
    Camera->Film.Dist = 1.0f;

    entity Entity = {};

    Entity.IsShape = true;
    Entity.Shape.Type = SPHERE;
    Entity.Shape.Material.Diffuse = rgb(93, 176, 244);
    Entity.Shape.Radius = 1.5f;
    Entity.Offset = vec3(0.0f, 0.0f, 4.5f);
    Scene->Entities.PushBack(Entity);

    //ground
    Entity.IsShape = true;
    Entity.Shape.Type = PLANE;
    Entity.Shape.Material.Diffuse = rgb(180, 180, 180);
    Entity.Shape.pPlane = vec3(0.0, -1.5f, 0.0);
    Entity.Shape.nPlane = vec3(0.0f, 1.0f, 0.0f);
    Scene->Entities.PushBack(Entity);    

    //Light
    entity Light = {};
    Light.IsShape = true;
    Light.Shape.Type = SPHERE;    
    Light.IsEmitter = true;    
    Light.Shape.Radius = 2.0f;
    Light.Emission.Flux = 50.0f;
    Light.Emission.Color = rgb(255, 255, 255);
    Light.Offset = vec3(5.0f, 8.0f, 5.0f);    
    Scene->Entities.PushBack(Light);
}

void
DemoScene3(camera *Camera, scene *Scene) {
    DemoScene2(Camera, Scene);

    entity Entity = {};
    Entity.IsShape = true;
    Entity.Shape.Type = PLANE;
    
    //left wall
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.pPlane = vec3(-4.0, 0.0f, 0.0);
    Entity.Shape.nPlane = vec3(1.0f, 0.0f, 0.0f);
    Scene->Entities.PushBack(Entity);

    //Right wall
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.pPlane = vec3(4.0, 0.0f, 0.0);
    Entity.Shape.nPlane = vec3(-1.0f, 0.0f, 0.0f);
    Scene->Entities.PushBack(Entity);

    //front wall
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.pPlane = vec3(0.0, 0.0f, 8.0);
    Entity.Shape.nPlane = vec3(0.0f, 0.0f, -1.0f);
    Scene->Entities.PushBack(Entity);

    //back wall
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.pPlane = vec3(0.0, 0.0f, -2.0);
    Entity.Shape.nPlane = vec3(0.0f, 0.0f, 1.0f);
    Scene->Entities.PushBack(Entity);

    //ceiling
    Entity.Shape.pPlane = vec3(0.0, 10.0f, 0.0);
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.nPlane = vec3(0.0f, -1.0f, 0.0f);
    Scene->Entities.PushBack(Entity);    
}

void 
DemoScene4(camera *Camera, scene *Scene) {
    Camera->Pos = vec3(0.0f, 0.0f, -3.0f);
    Camera->Dir = vec3(0.0f, 0.0f, 1.0f);
    Camera->Up = vec3(0.0f, 1.0f, 0.0f);
    Camera->Right = vec3(1.0f, 0.0f, 0.0f);
    Camera->Film.Dist = 1.0f;
    
    entity Entity = {};
    Entity.IsShape = true;
    Entity.Shape.Type = PLANE;
    
    //left wall
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.pPlane = vec3(-4.0, 0.0f, 0.0);
    Entity.Shape.nPlane = vec3(1.0f, 0.0f, 0.0f);
    Scene->Entities.PushBack(Entity);

    //Right wall
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.pPlane = vec3(4.0, 0.0f, 0.0);
    Entity.Shape.nPlane = vec3(-1.0f, 0.0f, 0.0f);
    Scene->Entities.PushBack(Entity);

    //front wall
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.pPlane = vec3(0.0, 0.0f, 8.0);
    Entity.Shape.nPlane = vec3(0.0f, 0.0f, -1.0f);
    Scene->Entities.PushBack(Entity);

    //back wall
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.pPlane = vec3(0.0, 0.0f, -4.0);
    Entity.Shape.nPlane = vec3(0.0f, 0.0f, 1.0f);
    Scene->Entities.PushBack(Entity);

    //ground
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.pPlane = vec3(0.0, -4.0f, 0.0);
    Entity.Shape.nPlane = vec3(0.0f, 1.0f, 0.0f);
    Scene->Entities.PushBack(Entity);
    
    //ceiling
    Entity.Shape.pPlane = vec3(0.0, 4.0f, 0.0);
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.nPlane = vec3(0.0f, -1.0f, 0.0f);
    Scene->Entities.PushBack(Entity);

    //Lights
    entity Light = {};
    Light.IsShape = true;
    Light.Shape.Type = SPHERE;    
    Light.IsEmitter = true;    
    Light.Shape.Radius = 1.0f;
    Light.Emission.Flux = 1000.0f;

    Light.Emission.Color = rgb(255, 0, 0);    
    Light.Offset = vec3(3.0f, 1.5f, 3.0f);    
    Scene->Entities.PushBack(Light);

    Light.Emission.Color = rgb(0, 255, 0);    
    Light.Offset = vec3(2.2f, -2.2f, 5.8f);    
    Scene->Entities.PushBack(Light);

    Light.Emission.Color = rgb(0, 0, 255);    
    Light.Offset = vec3(-2.5f, 2.5f, 4.0f);    
    Scene->Entities.PushBack(Light);

    Light.Emission.Color = rgb(255, 255, 255);    
    Light.Offset = vec3(-3.0f, -1.0f, 5.0f);    
    Scene->Entities.PushBack(Light);
}

void 
DemoScene5(camera *Camera, scene *Scene) {
    Camera->Pos = vec3(-0.5f, 0.0f, 8.0f);
    Camera->Dir = vec3(0.0f, 0.0f, -1.0f);
    Camera->Up = vec3(0.0f, 1.0f, 0.0f);
    Camera->Right = vec3(1.0f, 0.0f, 0.0f);
    Camera->Film.Dist = 1.0f;

    entity Entity = {};
    Entity.IsShape = false;
    Entity.Model = LoadObj("cube.obj");
    Entity.Offset = vec3(-0.5f, 0.0, 4.0f);
    Entity.Model.Materials[0].Diffuse = rgb(200, 250, 255);
    Scene->Entities.PushBack(Entity);


    //ground
    Entity.IsShape = true;
    Entity.Shape.Type = PLANE;
    Entity.Shape.Material.Diffuse = rgb(102, 255, 153);
    Entity.Shape.pPlane = vec3(0.0, -2.5f, 0.0);
    Entity.Shape.nPlane = normalize(vec3(0.0f, 1.0f, 0.0f));
    Scene->Entities.PushBack(Entity);

    //left wall
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.pPlane = vec3(-4.0, 0.0f, 0.0);
    Entity.Shape.nPlane = vec3(1.0f, 0.0f, 0.0f);
    Scene->Entities.PushBack(Entity);

    //Right wall
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.pPlane = vec3(4.0, 0.0f, 0.0);
    Entity.Shape.nPlane = vec3(-1.0f, 0.0f, 0.0f);
    Scene->Entities.PushBack(Entity);

    //front wall
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.pPlane = vec3(0.0, 0.0f, 8.0);
    Entity.Shape.nPlane = vec3(0.0f, 0.0f, -1.0f);
    Scene->Entities.PushBack(Entity);

    //back wall
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.pPlane = vec3(0.0, 0.0f, -2.0);
    Entity.Shape.nPlane = vec3(0.0f, 0.0f, 1.0f);
    Scene->Entities.PushBack(Entity);

    //ceiling
    Entity.Shape.pPlane = vec3(0.0, 10.0f, 0.0);
    Entity.Shape.Material.Diffuse = rgb(255, 255, 255);
    Entity.Shape.nPlane = vec3(0.0f, -1.0f, 0.0f);
    Scene->Entities.PushBack(Entity);    
    
    //Light
    entity Light = {};
    Light.IsShape = true;
    Light.Shape.Type = SPHERE;    
    Light.IsEmitter = true;    
    Light.Shape.Radius = 4.0f;
    Light.Emission.Flux = 100.0f;
    Light.Emission.Color = rgb(255, 255, 255);
    Light.Offset = vec3(2.0f, 8.0f, 2.0f);    
    Scene->Entities.PushBack(Light);
}


