
#include <time.h>
#include <mutex>
#include <thread>

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
    f32 transparency;
    f32 refractionN;
    f32 reflectivity;    
    u32 illumModel; //TODO: get rid of this
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
    vec3 skyColor;
    bool updated;
};

void
InitCamera(Camera *camera, u32 windowWidth, u32 windowHeight) {
    camera->pos = vec3(0.0f, 0.0f, 0.0f);
    camera->dir = vec3(0.0f, 0.0f, 1.0f);
    camera->up = vec3(0.0f, 1.0f, 0.0f);
    camera->right = vec3(1.0f, 0.0f, 0.0f);
            
    camera->film.pixelWidth = windowWidth;
    camera->film.pixelHeight = windowHeight;
    camera->film.buffer = (void *)malloc(camera->film.pixelWidth * camera->film.pixelHeight * sizeof(u32));
    camera->film.sumBuffer = (void *)calloc(camera->film.pixelWidth * camera->film.pixelHeight * sizeof(u32) * 4, 1);
    f32 aspect = (f32)windowWidth / (f32)windowHeight;
    camera->film.worldSize = vec2(aspect, 1.0f);
    camera->film.dist = 1.0f;
}

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
rgb(u32 r, u32 g, u32 b) {
    return vec3((f32)r / 255.0f, (f32)g / 255.0f, (f32)b / 255.0f);
}

inline vec3
GenRandomRay(vec3 n, vec3 reflection, f32 rCoefficient) {
    vec3 rd = normalize(vec3(((f32)g_RNG.rand_u32() / (f32)UINT_MAX) * 2.0f - 1.0f,
                             ((f32)g_RNG.rand_u32() / (f32)UINT_MAX) * 2.0f - 1.0f,
                             ((f32)g_RNG.rand_u32() / (f32)UINT_MAX) * 2.0f - 1.0f));
    if(dot(n, rd) < 0.0f)
        rd = rd * -1.0f;
    return lerp(rd, reflection, rCoefficient);
}

void
Draw(Camera *camera, Scene *scene) {

    int bounceCount = 25;
    if(camera->updated || scene->updated) {
        memset(camera->film.sumBuffer, (u8)0, camera->film.pixelWidth * camera->film.pixelHeight * sizeof(u32) * 4);
        camera->sampleCount = 0;
        camera->updated = false;
        scene->updated = false;
    }

    u32 *drawnPixelMap = (u32 *)calloc(camera->film.pixelWidth * camera->film.pixelHeight, sizeof(u32));
    for(int y = 0; y < camera->film.pixelHeight; y++) {    
        for(int x = 0; x < camera->film.pixelWidth; x++) {
            //choose a random unused point on the film
            u32 randX = g_RNG.rand_u32() % camera->film.pixelWidth;
            u32 randY = g_RNG.rand_u32() % camera->film.pixelHeight;
            u32 film_index = (randY * camera->film.pixelWidth) + randX;
            while(drawnPixelMap[film_index]) {
                randX = g_RNG.rand_u32() % camera->film.pixelWidth;
                randY = g_RNG.rand_u32() % camera->film.pixelHeight;
                film_index = randY * camera->film.pixelWidth + randX;
            }
            drawnPixelMap[film_index] = 1;
            
            f32 xDist = ((randX / (f32)camera->film.pixelWidth) - 0.5f) * camera->film.worldSize.x;
            f32 yDist = ((randY / (f32)camera->film.pixelHeight) - 0.5f) * camera->film.worldSize.y;
            vec3 pFilm = camera->pos + (camera->dir * camera->film.dist) + (camera->right * xDist) + (camera->up * yDist);
            vec3 ro = camera->pos;
            vec3 rd = normalize(pFilm - camera->pos);

            vec3 final_color = vec3(1.0f, 1.0f, 1.0f);
            int bounce = 0;
            bool hitLight = false;

            vec3 pCollision = vec3();
            vec3 nCollision = vec3();
            Material matCollision = {};
            while((bounce < bounceCount) && !hitLight) {
                f32 t = FLT_MAX;
                
                for(int i = 0; i < scene->entities.size; i++) {
                    
                    if(scene->entities[i].isShape) { //--- Shape ---
                        switch(scene->entities[i].shape.type) {
                            case SPHERE: {
                                f32 a = dot(rd, rd);
                                f32 b = 2.0f * dot(rd, ro - scene->entities[i].pos);
                                f32 c = dot((ro - scene->entities[i].pos), (ro - scene->entities[i].pos)) - (scene->entities[i].shape.radius * scene->entities[i].shape.radius);

                                f32 discriminant = b*b - 4.0f*a*c;
                                if(discriminant >= 0) {
                                    f32 t1 = (-b - (f32)sqrt(discriminant)) / 2.0f * a;
                                    f32 t2 = (-b + (f32)sqrt(discriminant)) / 2.0f * a;
                                    if((t1 > 0.0001f) || (t2 > 0.0001f)) {
                                        f32 t_temp = pos_min(t1, t2);
                                        if(t_temp < t) {
                                            t = t_temp;
                                            pCollision = ro + t*rd;               
                                            nCollision = normalize(pCollision - scene->entities[i].pos);
                                            matCollision = scene->entities[i].shape.material;
                                        }
                                    }
                                }
                                
                            } break;

                            case CUBE: {
                                //TODO
                            } break;

                            case PLANE: {
                                f32 t_temp = dot((scene->entities[i].shape.pPlane - ro), scene->entities[i].shape.nPlane) /
                                    dot(rd, scene->entities[i].shape.nPlane);
                                if((t_temp > 0.0001f) && (t_temp < t)) {
                                    t = t_temp;
                                    pCollision = ro + t*rd;
                                    nCollision = scene->entities[i].shape.nPlane;
                                    matCollision = scene->entities[i].shape.material;
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
                vec3 reflection = (dot(-rd, nCollision) * 2.0f * nCollision) + rd;
                f32 rand = (f32)g_RNG.rand_u32() / (f32)UINT_MAX;
                if(rand > matCollision.transparency) { //"transparency" is treated as a probability
                    rd = GenRandomRay(nCollision, reflection, matCollision.reflectivity);
                    final_color *= matCollision.diffuse;                
                }
                
                bounce++;
            }

            final_color = vec3((f32)pow((double)final_color.x, (double)(1.0/2.2)), //gamma correction
                               (f32)pow((double)final_color.y, (double)(1.0/2.2)),
                               (f32)pow((double)final_color.z, (double)(1.0/2.2)));
            //final_color = final_color / (1.0f + final_color);
            u32 color = get_u32_color(final_color * scene->skyColor);
            ((u32 *)camera->film.sumBuffer)[(film_index * 4) + 0] += (color >> 0) & 0xFF;
            ((u32 *)camera->film.sumBuffer)[(film_index * 4) + 1] += (color >> 8) & 0xFF;
            ((u32 *)camera->film.sumBuffer)[(film_index * 4) + 2] += (color >> 16) & 0xFF;
            ((u32 *)camera->film.sumBuffer)[(film_index * 4) + 3] += (color >> 24) & 0xFF;
        }
    }
    camera->sampleCount++;
    u32 totalPixelChannels = camera->film.pixelWidth * camera->film.pixelHeight * 4;
    for(u32 i = 0; i < totalPixelChannels; i++) {
        ((u8 *)camera->film.buffer)[i] = (u8) ((f32)(((u32 *)camera->film.sumBuffer)[i]) / (f32)camera->sampleCount);
    }

    delete drawnPixelMap;
}


