
struct vertex_attribute {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
};

struct material {
    vec3 Diffuse;
    vec3 Specular;
    f32 Exponent;
    f32 Transparency;
    f32 RefractionN;
};

struct model {
    char StrName[128];
    array<vertex_attribute> VertexAttributes;
    array<material> Materials;
    array<int> MaterialBases;
    array<int> MaterialSizes;
};

enum shape { SPHERE, PLANE };
struct primitive {
    shape Type;
    material Material;
    f32 Radius;
    vec3 nPlane;
    vec3 pPlane;
};

struct emission_profile {
    f32 Flux;
    vec3 Color;
};

struct entity {
    bool IsShape;
    bool IsEmitter;
    emission_profile Emission;
    model Model;
    primitive Shape;
    vec3 Offset;
};

struct scene {
    array<entity> Entities;
    bool Updated;
};

struct collision_info {
    vec3 p;
    vec3 n;
    material EntityMat = {};
    u32 EntityIndex;
    f32 t;
};

