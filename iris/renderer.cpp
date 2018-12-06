
#include <time.h>

struct Film {
    void *buffer;
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

enum Shape { SPHERE, CUBE, PLANE };
struct Primitive {
    Shape type;
    Material material;
    f32 radius;
    vec3 nPlane;
    vec3 pPlane;
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

inline vec3
GenRandomRay(vec3 n) {
    vec3 r = normalize(vec3(((f32)g_RNG.rand_u32() / (f32)UINT_MAX) * 2.0f - 1.0f,
                            ((f32)g_RNG.rand_u32() / (f32)UINT_MAX) * 2.0f - 1.0f,
                            ((f32)g_RNG.rand_u32() / (f32)UINT_MAX) * 2.0f - 1.0f));
    if(dot(n, r) < 0.0f)
        r = r * -1.0f;
    return r;
}

inline vec3
ReflectionRay(vec3 r, vec3 n) {
    return normalize(-r + n * 2.0f * dot(r, n));
}

bool
Sample(Camera camera, Scene scene) {
    int bounceCount = 8;
    u32 *pixel = (u32 *)camera.film.buffer;
    
    for(int y = 0; y < camera.film.pixelHeight; y++) {    
        for(int x = 0; x < camera.film.pixelWidth; x++) {
            f32 xDist = ((x / (f32)camera.film.pixelWidth) - 0.5f) * camera.film.worldSize.x;
            f32 yDist = ((y / (f32)camera.film.pixelHeight) - 0.5f) * camera.film.worldSize.y;
            vec3 pFilm = camera.pos + (camera.dir * camera.film.dist) + (camera.right * xDist) + (camera.up * yDist);
            vec3 ro = camera.pos;
            vec3 rd = normalize(pFilm - camera.pos);

            vec3 final_color = vec3(1.0f, 1.0f, 1.0f);
            int bounce = 0;
            bool hitLight = false;
            
            while((bounce < bounceCount) && !hitLight) {
                vec3 pCollision = vec3();
                vec3 nCollision = vec3();
                Material matCollision = {};
                f32 t = FLT_MAX;
                
                for(int i = 0; i < scene.entities.size; i++) {
                    if(scene.entities[i].isShape) { //--- Shape ---
                        switch(scene.entities[i].shape.type) {
                            case SPHERE: {
                                f32 a = dot(rd, rd);
                                f32 b = 2.0f * dot(rd, ro - scene.entities[i].pos);
                                f32 c = dot((ro - scene.entities[i].pos), (ro - scene.entities[i].pos)) - (scene.entities[i].shape.radius * scene.entities[i].shape.radius);

                                f32 discriminant = b*b - 4.0f*a*c;
                                if(discriminant >= 0) {
                                    f32 t1 = (-b - (f32)sqrt(discriminant)) / 2.0f * a;
                                    f32 t2 = (-b + (f32)sqrt(discriminant)) / 2.0f * a;
                                    if((t1 > 0.0001f) || (t2 > 0.0001f)) {
                                        f32 t_temp = pos_min(t1, t2);
                                        if(t_temp < t) {
                                            t = t_temp;
                                            pCollision = ro + t*rd;                                         
                                            nCollision = normalize(pCollision - scene.entities[i].pos);
                                            matCollision = scene.entities[i].shape.material;
                                        }
                                    }
                                }
                                
                            } break;

                            case CUBE: {
                                //TODO
                            } break;

                            case PLANE: {
                                f32 t_temp = dot((scene.entities[i].shape.pPlane - ro), scene.entities[i].shape.nPlane) /
                                    dot(rd, scene.entities[i].shape.nPlane);
                                if((t_temp > 0.0f) && (t_temp < t)) {
                                    t = t_temp;
                                    pCollision = ro + t*rd;
                                    nCollision = scene.entities[i].shape.nPlane;
                                    matCollision = scene.entities[i].shape.material;
                                }                               
                                
                            } break;
                                
                            default: {
                            } break;
                        }
                    } else { //--- Model ---
                        //TODO: Triangle intersection tests
                    }
                }
                

                if(t == FLT_MAX)
                    break;
                
                ro = pCollision;
                rd = GenRandomRay(nCollision);
                final_color *= matCollision.diffuse;
                
                bounce++;
            }
            *pixel++ = get_u32_color(final_color * vec3(0.8f, 0.82f, 0.92f));
        }
    }
    return true;
}

bool
Draw(Camera camera, Scene scene, u32 sampleCount) {
    u32 sizeMultiplier = 4; //times larger to make the buffer holding the sum of samples to prevent overflow
    u32 sumBufferSize = camera.film.pixelWidth * camera.film.pixelHeight * sizeof(u32) * sizeMultiplier;
    void *sumBuffer = (void *)calloc(sumBufferSize, 1);
    for(u32 i = 0; i < sampleCount; i++) {
        if(!Sample(camera, scene))
            return false;

        for(u32 i = 0; i < sumBufferSize / sizeMultiplier; i++) {
            ((u32 *)sumBuffer)[i] += ((u8 *)camera.film.buffer)[i];
        }
    }

    for(u32 i = 0; i < sumBufferSize / sizeMultiplier; i++) {
        ((u8 *)camera.film.buffer)[i] = (u8)(((u32 *)sumBuffer)[i] / sampleCount);
    }
    
    delete sumBuffer;
    return true;
}

