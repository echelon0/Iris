
void
DemoScene1(Camera *camera, Scene *scene) {
    camera->pos = vec3(0.0f, 0.0f, 0.0f);
    camera->dir = vec3(0.0f, 0.0f, 1.0f);
    camera->up = vec3(0.0f, 1.0f, 0.0f);
    camera->right = vec3(1.0f, 0.0f, 0.0f);
    camera->film.dist = 1.2f;

    Entity entity = {};
    entity.isShape = true;
    entity.shape.type = SPHERE;
    entity.shape.material.diffuse = vec3(1.0f, 0.0f, 0.0f);         
    entity.shape.radius = 1.0f;
    entity.pos = vec3(-1.0f, -0.85f, 7.0f);
    scene->entities.PushBack(entity);

    entity.isShape = true;
    entity.shape.type = SPHERE;
    entity.shape.material.diffuse = vec3(0.0f, 1.0f, 0.0f);
    entity.shape.radius = 1.0f;
    entity.pos = vec3(0.0f, 0.9f, 7.0f);
    scene->entities.PushBack(entity);

    entity.isShape = true;
    entity.shape.type = SPHERE;
    entity.shape.material.diffuse = vec3(0.0f, 0.0f, 1.0f);
    entity.shape.radius = 1.0f;
    entity.pos = vec3(1.0f, -0.85f, 7.0f);
    scene->entities.PushBack(entity);

    entity.isShape = true;
    entity.shape.type = PLANE;
    entity.shape.material.diffuse = vec3(1.0f, 1.0f, 1.0f);
    entity.shape.pPlane = vec3(0.0, -2.0f, 0.0);
    entity.shape.nPlane = normalize(vec3(0.0f, 1.0f, 0.0f));
    scene->entities.PushBack(entity);

    entity.isShape = true;
    entity.shape.type = PLANE;
    entity.shape.material.diffuse = vec3(1.0f, 1.0f, 1.0f);
    entity.shape.pPlane = vec3(0.0, 0.0f, 8.5);
    entity.shape.nPlane = normalize(vec3(0.0f, 0.0f, -1.0f));
    scene->entities.PushBack(entity);

    entity.isShape = true;
    entity.shape.type = PLANE;
    entity.shape.material.diffuse = vec3(1.0f, 1.0f, 1.0f);
    entity.shape.pPlane = vec3(2.5, 0.0f, 0.0);
    entity.shape.nPlane = normalize(vec3(-1.0f, 0.0f, 0.0f));
    scene->entities.PushBack(entity);

    entity.isShape = true;
    entity.shape.type = PLANE;
    entity.shape.material.diffuse = vec3(1.0f, 1.0f, 1.0f);
    entity.shape.pPlane = vec3(-2.5, 0.0f, 0.0);
    entity.shape.nPlane = normalize(vec3(1.0f, 0.0f, 0.0f));
    scene->entities.PushBack(entity);

}
