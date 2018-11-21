

struct Film {
    void *buffer;
    int width;
    int height;

    float dist;
};

struct Camera {
    vec3 pos;
    vec3 dir;
    vec3 up;

    Film film;

//    AnimatedTransform cam_to_world;
//    shutter speed
//    apeture diameter
//    focus_distance
//    Film
//    Medium
};

struct VertexAttribute {
    vec3 position;
    vec3 normal;
    vec2 texcoord;
};

struct Material {
    vec3 diffuse;
    vec3 specular;
    f32 exponent;
    f32 dissolve;
    u32 illumModel;
};

struct Model {
    char strName[128];
    Array<VertexAttribute> vertexAttributes;
    Array<Material> materials;
    Array<int> materialBases;
    Array<int> materialSizes;
};

struct Entity {
    Model model;
    vec3 pos;
};

struct Scene {
    Array<Entity> entities;
};

void Sample(Camera camera, Scene scene) {
    bool hasAlpha = false;
    if(sizeof(camera.film.buffer) == (sizeof(u8) * 4 * width * height)) {
        hasAlpha = true;
    } else if(sizeof(camera.film.buffer) != (sizeof(u8) * 3 * width * height)) {
        LOG_ERROR("ERROR", "Invalid film buffer size.");
        return;
    }
    
    
}

