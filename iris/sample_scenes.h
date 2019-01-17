
void
DemoScene1(Camera *camera, Scene *scene) {
    camera->pos = vec3(0.0f, 0.0f, 0.0f);
    camera->dir = vec3(0.0f, 0.0f, 1.0f);
    camera->up = vec3(0.0f, 1.0f, 0.0f);
    camera->right = vec3(1.0f, 0.0f, 0.0f);
    camera->film.dist = 1.0f;

    scene->skyColor = rgb(255, 255, 255);

    Entity entity = {};

    f32 sphereSpacing = 1.15f;

    //bottom left sphere
    entity.isShape = true;
    entity.shape.type = SPHERE;
    entity.shape.material.diffuse = rgb(255, 25, 0);
    entity.shape.material.reflectivity = 0.8f;    
    entity.shape.radius = 1.0f;
    entity.offset = vec3(-(f32)sqrt(3.0f)/2.0f, -1.0f/2.0f, 5.0f) * sphereSpacing;
    scene->entities.PushBack(entity);

    //bottom right sphere
    entity.isShape = true;
    entity.shape.type = SPHERE;
    entity.shape.material.diffuse = rgb(255, 255, 102);
    entity.shape.material.reflectivity = 0.5f;    
    entity.shape.radius = 1.0f;
    entity.offset = vec3((f32)sqrt(3.0f)/2.0f, -1.0f/2.0f, 5.0f) * sphereSpacing;
    scene->entities.PushBack(entity);

    
    //top middle sphere
    entity.isShape = true;
    entity.shape.type = SPHERE;
    entity.shape.material.diffuse = rgb(255, 102, 102);
    entity.shape.material.reflectivity = 0.5f;        
    entity.shape.radius = 1.0f;
    entity.offset = vec3(0.0f, 1.0f, 5.0f) * sphereSpacing;
    scene->entities.PushBack(entity);

    //middle sphere
    entity.isShape = true;
    entity.shape.type = SPHERE;
    entity.shape.material.diffuse = rgb(255, 255, 255);
    entity.shape.material.reflectivity = 0.0f;
    entity.shape.material.transparency = 0.8f;
    entity.shape.material.refractionN = 1.090f;
    entity.shape.radius = 0.5f;
    entity.offset = vec3(0.0f, 0.0f, 4.5f);
    scene->entities.PushBack(entity);

    //bottom
    entity.isShape = true;
    entity.shape.type = PLANE;
    entity.shape.material.diffuse = rgb(102, 255, 153);
    entity.shape.material.reflectivity = 0.8f;
    entity.shape.material.transparency = 0.0f;    
    entity.shape.pPlane = vec3(0.0, -2.5f, 0.0);
    entity.shape.nPlane = normalize(vec3(0.0f, 1.0f, 0.0f));
    scene->entities.PushBack(entity);
    
//    entity.isShape = false;
//    entity.model = LoadObj("cube.obj");
//    entity.offset = vec3(2.0f, 1.0, 3.0f);
//    scene->entities.PushBack(entity);
}
