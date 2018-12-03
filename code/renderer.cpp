
struct Film {
    void *buffer;
    int pixelWidth;
    int pixelHeight;
    vec2 worldSize;
    float dist;
};

struct Camera {
    vec3 pos;
    vec3 dir;
    vec3 up;
    vec3 right;

    Film film;

//    AnimatedTransform cam_to_world;
//    shutter speed
//    apeture diameter
//    focus_distance
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

enum Shape { SPHERE, CUBE };
struct Primitive {
    Shape type;
    Material material;    
    float radius;    
};

struct Entity {
    bool isShape;
    union {
        Model model;
        Primitive shape;
    };
    vec3 pos;
};

struct Scene {
    Array<Entity> entities;
};

void
RenderBuffer(HDC deviceContext, void *buffer,
             int windowWidth, int windowHeight,
             int bufferWidth, int bufferHeight) {
    
    BITMAPINFO bitmapInfo = {};
    bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
    bitmapInfo.bmiHeader.biWidth = bufferWidth;
    bitmapInfo.bmiHeader.biHeight = bufferHeight;
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 32;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;
    
    StretchDIBits(deviceContext,
                  0, 0, windowWidth, windowHeight,
                  0, 0, bufferWidth, bufferHeight,
                  buffer,
                  &bitmapInfo,
                  DIB_RGB_COLORS,
                  SRCCOPY);
}

inline u32
get_u32_color(vec3 vec) {
    return (((u8)(vec.x * 255.0f) << 16) | ((u8)(vec.y * 255.0f) << 8) | ((u8)(vec.z * 255.0f) << 0));
}

bool
Sample(Camera camera, Scene scene) {
    int bounceCount = 2;
    
    u32 *pixel = (u32 *)camera.film.buffer;
    for(int y = 0; y < camera.film.pixelHeight; y++) {    
        for(int x = 0; x < camera.film.pixelWidth; x++) {
            
            float xDist = ((x / (f32)camera.film.pixelWidth) - 0.5f) * camera.film.worldSize.x;
            float yDist = ((y / (f32)camera.film.pixelHeight) - 0.5f) * camera.film.worldSize.y;
            vec3 pFilm = camera.pos + (camera.dir * camera.film.dist) + (camera.right * xDist) + (camera.up * yDist);
            vec3 ro = camera.pos;
            vec3 rd = normalize(pFilm - camera.pos);

            vec3 final_color = vec3(1.0f, 1.0f, 1.0f);
            bool hitLight = false;
            int bounce = 0;
            while((bounce < bounceCount) && !hitLight) {
                float t = FLT_MAX;
                vec3 pCollision = vec3();
                vec3 nCollision = vec3();
                Material matCollision = {};
                
                //--- Primitives ---
                for(int i = 0; i < scene.entities.size; i++) {
                    if(scene.entities[i].isShape) { //--- Shape ---
                        switch(scene.entities[i].shape.type) {
                            case SPHERE: {
                                float a = dot(rd, rd);
                                float b = 2.0f * dot(rd, ro - scene.entities[i].pos);
                                float c = dot((ro - scene.entities[i].pos), (ro - scene.entities[i].pos)) - (scene.entities[i].shape.radius * scene.entities[i].shape.radius);

                                float discriminant = b*b - 4.0f*a*c;
                                if(discriminant >= 0) {
                                    float t1 = (-b - (f32)sqrt(discriminant)) / 2.0f * a;
                                    float t2 = (-b + (f32)sqrt(discriminant)) / 2.0f * a;
                                    if((t1 > 0.0f) || (t2 > 0.0f)) {
                                        float t_temp = pos_min(t1, t2);
                                        if(t_temp < t) {
                                            t = t_temp;
                                            pCollision = ro + t*rd;                                         
                                            nCollision = normalize(pCollision - scene.entities[i].pos);
                                            matCollision = scene.entities[i].shape.material;
                                        }
                                    }
                                }
                                
                            } break;
                            
                            default: {
                            } break;
                        }
                    } else { //--- Model ---
                    }
                }
                

                if(t == FLT_MAX)
                    break;
                
                ro = pCollision;
                rd = normalize(rd + nCollision * 2.0f * dot(-rd, nCollision));
                final_color *= matCollision.diffuse;
                
                bounce++;
            }
            *pixel++ = get_u32_color(final_color * vec3(0.8f, 0.82f, 0.92f));
        }
    }
    return true;
}

