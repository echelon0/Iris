
void
DemoScene1(Camera *camera, Scene *scene) {
    camera->pos = vec3(0.0f, 0.0f, 0.0f);
    camera->dir = vec3(0.0f, 0.0f, 1.0f);
    camera->up = vec3(0.0f, 1.0f, 0.0f);
    camera->right = vec3(1.0f, 0.0f, 0.0f);
    camera->film.dist = 1.0f;

    scene->skyColor = rgb(255, 255, 255);

    Entity entity = {};

    vec3 start = vec3(-3.0f, -0.5f, 5.5f);
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
    entity.shape.material.diffuse = rgb(255, 255, 255);
    entity.shape.pPlane = vec3(0.0, -1.5f, 0.0);
    entity.shape.nPlane = vec3(0.0f, 1.0f, 0.0f);
    scene->entities.PushBack(entity);

    //light
    Entity light = {};
    light.isShape = true;
    light.shape.type = SPHERE;    
    light.isEmitter = true;    
    light.shape.radius = 0.5f;
    light.emission.flux = 100.0f;
    light.emission.color = rgb(255, 255, 255);
    light.offset = vec3(0.0f, 3.0f, 0.0f);
    scene->entities.PushBack(light);
}

void 
DemoScene2(Camera *camera, Scene *scene) {
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

    //ground
    entity.isShape = true;
    entity.shape.type = PLANE;
    entity.shape.material.diffuse = rgb(102, 255, 153);
    entity.shape.pPlane = vec3(0.0, -2.5f, 0.0);
    entity.shape.nPlane = normalize(vec3(0.0f, 1.0f, 0.0f));
    scene->entities.PushBack(entity);

}
