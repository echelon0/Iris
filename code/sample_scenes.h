
void
DemoScene1(Camera *camera, Scene *scene) {
    camera->pos = vec3(0.0f, 0.0f, 0.0f);
    camera->dir = vec3(0.0f, 0.0f, 1.0f);
    camera->up = vec3(0.0f, 1.0f, 0.0f);
    camera->right = vec3(1.0f, 0.0f, 0.0f);
    camera->film.dist = 1.0f;

    scene->skyColor = rgb(255, 255, 255);

    Entity entity = {};

    vec3 start = vec3(-3.0f, -0.5f, 6.0f);
    entity.isShape = true;
    entity.shape.type = SPHERE;
    entity.shape.material.diffuse = rgb(255, 0, 0);
    entity.shape.radius = 1.0f;
    entity.offset = start;
    scene->entities.PushBack(entity);

    vec3 spacing = vec3((2.0f * absf(start.x) / 3.0f), 0.0f, 0.0f);

    entity.shape.material.diffuse = rgb(0, 255, 0);
    entity.offset += spacing;
    scene->entities.PushBack(entity);

    entity.shape.material.diffuse = rgb(0, 0, 255);
    entity.offset += spacing;
    scene->entities.PushBack(entity);

    entity.shape.material.diffuse = rgb(255, 255, 255);
    entity.offset += spacing;
    scene->entities.PushBack(entity);

    //ground
    entity.isShape = true;
    entity.shape.type = PLANE;
    entity.shape.material.diffuse = rgb(180, 180, 180);
    entity.shape.pPlane = vec3(0.0, -1.5f, 0.0);
    entity.shape.nPlane = vec3(0.0f, 1.0f, 0.0f);
    scene->entities.PushBack(entity);

    //left wall
    entity.shape.material.diffuse = rgb(198, 192, 135);
    entity.shape.pPlane = vec3(-4.0, 0.0f, 0.0);
    entity.shape.nPlane = vec3(1.0f, 0.0f, 0.0f);
    scene->entities.PushBack(entity);

    //right wall
    entity.shape.material.diffuse = rgb(198, 192, 135);
    entity.shape.pPlane = vec3(4.0, 0.0f, 0.0);
    entity.shape.nPlane = vec3(-1.0f, 0.0f, 0.0f);
    scene->entities.PushBack(entity);

    //front wall
    entity.shape.material.diffuse = rgb(255, 255, 255);
    entity.shape.pPlane = vec3(0.0, 0.0f, 8.0);
    entity.shape.nPlane = vec3(0.0f, 0.0f, -1.0f);
    scene->entities.PushBack(entity);

    //back wall
    entity.shape.material.diffuse = rgb(180, 180, 180);
    entity.shape.pPlane = vec3(0.0, 0.0f, -2.0);
    entity.shape.nPlane = vec3(0.0f, 0.0f, 1.0f);
    scene->entities.PushBack(entity);

    //light
    Entity light = {};
    light.isShape = true;
    light.shape.type = SPHERE;    
    light.isEmitter = true;    
    light.shape.radius = 2.5f;
    light.emission.flux = 10000.0f;
    light.emission.color = rgb(255, 255, 255);
    light.offset = vec3(0.0f, 4.0f, 0.0f);    
    scene->entities.PushBack(light);
}

void 
DemoScene2(Camera *camera, Scene *scene) {
    camera->pos = vec3(0.0f, 0.0f, 0.0f);
    camera->dir = vec3(0.0f, 0.0f, 1.0f);
    camera->up = vec3(0.0f, 1.0f, 0.0f);
    camera->right = vec3(1.0f, 0.0f, 0.0f);
    camera->film.dist = 1.0f;

    scene->skyColor = rgb(255, 255, 255);

    Entity entity = {};

    entity.isShape = true;
    entity.shape.type = SPHERE;
    entity.shape.material.diffuse = rgb(93, 176, 244);
    entity.shape.radius = 1.5f;
    entity.offset = vec3(0.0f, 0.0f, 6.0f);
    scene->entities.PushBack(entity);

    //ground
    entity.isShape = true;
    entity.shape.type = PLANE;
    entity.shape.material.diffuse = rgb(180, 180, 180);
    entity.shape.pPlane = vec3(0.0, -1.5f, 0.0);
    entity.shape.nPlane = vec3(0.0f, 1.0f, 0.0f);
    scene->entities.PushBack(entity);    

    //light
    Entity light = {};
    light.isShape = true;
    light.shape.type = SPHERE;    
    light.isEmitter = true;    
    light.shape.radius = 3.5f;
    light.emission.flux = 1000.0f;
    light.emission.color = rgb(255, 255, 255);
    light.offset = vec3(2.0f, 8.0f, 4.0f);    
    scene->entities.PushBack(light);
}

void
DemoScene3(Camera *camera, Scene *scene) {
    DemoScene2(camera, scene);

    Entity entity = {};
    entity.isShape = true;
    entity.shape.type = PLANE;
    
    //left wall
    entity.shape.material.diffuse = rgb(255, 255, 255);
    entity.shape.pPlane = vec3(-4.0, 0.0f, 0.0);
    entity.shape.nPlane = vec3(1.0f, 0.0f, 0.0f);
    scene->entities.PushBack(entity);

    //right wall
    entity.shape.material.diffuse = rgb(255, 255, 255);
    entity.shape.pPlane = vec3(4.0, 0.0f, 0.0);
    entity.shape.nPlane = vec3(-1.0f, 0.0f, 0.0f);
    scene->entities.PushBack(entity);

    //front wall
    entity.shape.material.diffuse = rgb(255, 255, 255);
    entity.shape.pPlane = vec3(0.0, 0.0f, 8.0);
    entity.shape.nPlane = vec3(0.0f, 0.0f, -1.0f);
    scene->entities.PushBack(entity);

    //back wall
    entity.shape.material.diffuse = rgb(255, 255, 255);
    entity.shape.pPlane = vec3(0.0, 0.0f, -2.0);
    entity.shape.nPlane = vec3(0.0f, 0.0f, 1.0f);
    scene->entities.PushBack(entity);

    //ceiling
    entity.shape.pPlane = vec3(0.0, 10.0f, 0.0);
    entity.shape.material.diffuse = rgb(255, 255, 255);
    entity.shape.nPlane = vec3(0.0f, -1.0f, 0.0f);
    scene->entities.PushBack(entity);    
}

void 
DemoScene4(Camera *camera, Scene *scene) {
    camera->pos = vec3(0.0f, 0.0f, 0.0f);
    camera->dir = vec3(0.0f, 0.0f, -1.0f);
    camera->up = vec3(0.0f, 1.0f, 0.0f);
    camera->right = vec3(1.0f, 0.0f, 0.0f);
    camera->film.dist = 1.0f;

    scene->skyColor = rgb(255, 255, 255);

    Entity entity = {};
    entity.isShape = false;
    entity.model = LoadObj("monkey.obj");
    entity.offset = vec3(-0.5f, 0.0, -4.0f);
    scene->entities.PushBack(entity);

    /*
    //ground
    entity.isShape = true;
    entity.shape.type = PLANE;
    entity.shape.material.diffuse = rgb(102, 255, 153);
    entity.shape.pPlane = vec3(0.0, -2.5f, 0.0);
    entity.shape.nPlane = normalize(vec3(0.0f, 1.0f, 0.0f));
    scene->entities.PushBack(entity);
    
    //left wall
    entity.shape.material.diffuse = rgb(255, 255, 255);
    entity.shape.pPlane = vec3(-4.0, 0.0f, 0.0);
    entity.shape.nPlane = vec3(1.0f, 0.0f, 0.0f);
    scene->entities.PushBack(entity);

    //right wall
    entity.shape.material.diffuse = rgb(255, 255, 255);
    entity.shape.pPlane = vec3(4.0, 0.0f, 0.0);
    entity.shape.nPlane = vec3(-1.0f, 0.0f, 0.0f);
    scene->entities.PushBack(entity);

    //front wall
    entity.shape.material.diffuse = rgb(255, 255, 255);
    entity.shape.pPlane = vec3(0.0, 0.0f, 8.0);
    entity.shape.nPlane = vec3(0.0f, 0.0f, -1.0f);
    scene->entities.PushBack(entity);

    //back wall
    entity.shape.material.diffuse = rgb(255, 255, 255);
    entity.shape.pPlane = vec3(0.0, 0.0f, -2.0);
    entity.shape.nPlane = vec3(0.0f, 0.0f, 1.0f);
    scene->entities.PushBack(entity);

    //ceiling
    entity.shape.pPlane = vec3(0.0, 10.0f, 0.0);
    entity.shape.material.diffuse = rgb(255, 255, 255);
    entity.shape.nPlane = vec3(0.0f, -1.0f, 0.0f);
    scene->entities.PushBack(entity);    
    */
    
    //light
    Entity light = {};
    light.isShape = true;
    light.shape.type = SPHERE;    
    light.isEmitter = true;    
    light.shape.radius = 4.0f;
    light.emission.flux = 10000.0f;
    light.emission.color = rgb(255, 255, 255);
    light.offset = vec3(4.0f, 8.0f, -4.0f);    
    scene->entities.PushBack(light);
}
