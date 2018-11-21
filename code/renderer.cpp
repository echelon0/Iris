
struct Camera {
    vec3 pos;
    vec3 dir;
    vec3 up;

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

struct Scene {
    Camera camera;
    Array<Model> models;
};

void Sample(Scene scene, void *outBuffer, int width, int height) {
    
}
