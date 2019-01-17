
#include "renderer.h"

int
GetMaterialIndex(Model model, int vertexIndex) {
    for(int baseIndex = 0; baseIndex < model.materialBases.size; baseIndex++) {
        if(vertexIndex >= model.materialBases[baseIndex] &&
           (vertexIndex < model.materialBases[baseIndex]+ model.materialSizes[baseIndex])) {
            return baseIndex;
        }
    };
    return -1;
}

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
    int bounceCount = 8;
    if(camera->updated || scene->updated) {
        memset(camera->film.sumBuffer, (u8)0, camera->film.pixelWidth * camera->film.pixelHeight * sizeof(u32) * 4);
        camera->sampleCount = 0;
        camera->updated = false;
        scene->updated = false;
    }

    for(int y = 0; y < camera->film.pixelHeight; y++) {    
        for(int x = 0; x < camera->film.pixelWidth; x++) {
            f32 xDist = ((x / (f32)camera->film.pixelWidth) - 0.5f) * camera->film.worldSize.x;
            f32 yDist = ((y / (f32)camera->film.pixelHeight) - 0.5f) * camera->film.worldSize.y;
            vec3 pFilm = camera->pos + (camera->dir * camera->film.dist) + (camera->right * xDist) + (camera->up * yDist);
            vec3 ro = camera->pos;
            vec3 rd = normalize(pFilm - camera->pos);

            int bounce = 0;
            bool hitLight = false;
            vec3 final_color = vec3(1.0f, 1.0f, 1.0f);      

            vec3 pCollision = vec3();
            vec3 nCollision = vec3();
            Material matCollision = {};
            while((bounce < bounceCount) && !hitLight) {
                f32 t = FLT_MAX;
                
                for(int entityIndex = 0; entityIndex < scene->entities.size; entityIndex++) {
                    Entity currentEntity = scene->entities[entityIndex];
                    if(currentEntity.isShape) { //--- Shape ---
                        switch(currentEntity.shape.type) {
                            case SPHERE: {
                                f32 a = dot(rd, rd);
                                f32 b = 2.0f * dot(rd, ro - currentEntity.offset);
                                f32 c = dot((ro - currentEntity.offset), (ro - currentEntity.offset)) - (currentEntity.shape.radius * currentEntity.shape.radius);

                                f32 discriminant = b*b - 4.0f*a*c;
                                if(discriminant >= 0) {
                                    f32 t1 = (-b - (f32)sqrt(discriminant)) / 2.0f * a;
                                    f32 t2 = (-b + (f32)sqrt(discriminant)) / 2.0f * a;
                                    if((t1 > 0.0001f) || (t2 > 0.0001f)) {
                                        f32 t_temp = pos_min(t1, t2);
                                        if(t_temp < t) {
                                            t = t_temp;
                                            pCollision = ro + t*rd;               
                                            nCollision = normalize(pCollision - currentEntity.offset);
                                            matCollision = currentEntity.shape.material;
                                        }
                                    }
                                }
                                
                            } break;

                            case PLANE: {
                                f32 t_temp = dot((currentEntity.shape.pPlane - ro), currentEntity.shape.nPlane) /
                                    dot(rd, currentEntity.shape.nPlane);
                                if((t_temp > 0.0001f) && (t_temp < t) && (dot(currentEntity.shape.nPlane, -rd) > 0.0f)) {
                                    t = t_temp;
                                    pCollision = ro + t*rd;
                                    nCollision = currentEntity.shape.nPlane;
                                    matCollision = currentEntity.shape.material;
                                }                               
                                
                            } break;
                                
                            default: {
                            } break;
                        }
                    } else { //--- Model ---
                        for(int baseVertex = 0; baseVertex < currentEntity.model.vertexAttributes.size; baseVertex+=3) {
                            vec3 n = currentEntity.model.vertexAttributes[baseVertex].normal;                          
                            vec3 v0 = currentEntity.model.vertexAttributes[baseVertex + 0].position + currentEntity.offset;
                            vec3 v1 = currentEntity.model.vertexAttributes[baseVertex + 1].position + currentEntity.offset;
                            vec3 v2 = currentEntity.model.vertexAttributes[baseVertex + 2].position + currentEntity.offset;

                            vec3 intersection;                      
                            f32 t_temp;
                            if(ray_intersects_triangle(ro, rd, v0, v1, v2, n, t_temp)) {
                                if((t_temp > 0.0001f) && (t_temp < t) && (dot(n, rd) > 0.0f)) {
                                    t = t_temp;
                                    pCollision = intersection;
                                    nCollision = n;
                                    matCollision = currentEntity.model.materials[GetMaterialIndex(currentEntity.model, baseVertex)];
                                }
                            }
                        }
                    }
                }
                
                if(t == FLT_MAX)
                    break;
                
                ro = pCollision;
                vec3 reflection = (dot(-rd, nCollision) * 2.0f * nCollision) + rd;
                rd = GenRandomRay(nCollision, reflection, 0.5f);

                final_color *= matCollision.diffuse;

                bounce++;
            }

            final_color = vec3((f32)pow((double)final_color.x, (double)(1.0/2.2)), //gamma correction
                               (f32)pow((double)final_color.y, (double)(1.0/2.2)),
                               (f32)pow((double)final_color.z, (double)(1.0/2.2)));
            u32 color = get_u32_color(final_color * scene->skyColor);
            u32 filmIndex = (y * camera->film.pixelWidth) + x;     
            ((u32 *)camera->film.sumBuffer)[(filmIndex * 4) + 0] += (color >> 0) & 0xFF;
            ((u32 *)camera->film.sumBuffer)[(filmIndex * 4) + 1] += (color >> 8) & 0xFF;
            ((u32 *)camera->film.sumBuffer)[(filmIndex * 4) + 2] += (color >> 16) & 0xFF;
            ((u32 *)camera->film.sumBuffer)[(filmIndex * 4) + 3] += (color >> 24) & 0xFF;
        }
    }
    camera->sampleCount++;
    u32 totalPixelChannels = camera->film.pixelWidth * camera->film.pixelHeight * 4;
    for(u32 i = 0; i < totalPixelChannels; i++) {
        ((u8 *)camera->film.buffer)[i] = (u8) ((f32)(((u32 *)camera->film.sumBuffer)[i]) / (f32)camera->sampleCount);
    }

}


