
struct Film {
    void *buffer;
    void *sumBuffer;    
    int pixelWidth;
    int pixelHeight;
    vec2 worldSize;
    f32 dist;
};

struct Camera {
    vec3 pos;
    vec3 dir;
    vec3 up;
    vec3 right;

    Film film;
    u32 sampleCount;
    bool updated;
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
    f32 transparency;
    f32 refractionN;
};

struct Model {
    char strName[128];
    Array<VertexAttribute> vertexAttributes;
    Array<Material> materials;
    Array<int> materialBases;
    Array<int> materialSizes;
};

enum Shape { SPHERE, PLANE };
struct Primitive {
    Shape type;
    Material material;
    f32 radius;
    vec3 nPlane;
    vec3 pPlane;
};

struct EmissionProfile {
    f32 flux;
    vec3 color;
};

struct Entity {
    bool isShape;
    bool isEmitter;
    EmissionProfile emission;
    union {
        Model model;
        Primitive shape;
    };
    vec3 offset;
};

struct Scene {
    Array<Entity> entities;
    vec3 skyColor;
    bool updated;
};

struct CollisionInfo {
    vec3 p;
    vec3 n;
    Material entityMat = {};
    u32 entityIndex;
    f32 t;
};

int GetMaterialIndex(Model model, int vertexIndex);
void InitCamera(Camera *camera, u32 windowWidth, u32 windowHeight);
void RenderBuffer(HDC deviceContext, void *buffer,
                  int windowWidth, int windowHeight,
                  int bufferWidth, int bufferHeight);
inline u32 get_u32_color(vec3 vec);
inline vec3 rgb(u32 r, u32 g, u32 b);
inline vec3 GenRandomRay(vec3 n, vec3 reflection, f32 rCoefficient);
void Draw(Camera *camera, Scene *scene);
