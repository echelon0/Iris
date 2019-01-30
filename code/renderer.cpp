
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

vec2
UniformSampleDisk() {
    f32 r = absf((f32)cos((f32)g_RNG.rand_u32()));
    f32 theta = (f32)g_RNG.rand_u32();

    f32 x = r * (f32)cos(theta);
    f32 y = r * (f32)sin(theta);
    
    vec2 p = vec2(x, y);
    return p;
}

vec3
UniformSampleHemisphere(vec3 n) {
    f32 theta = (f32)g_RNG.rand_u32();
    f32 phi = (f32)g_RNG.rand_u32();
    f32 x = (f32)sin(theta) * (f32)cos(phi);
    f32 y = (f32)sin(theta) * (f32)sin(phi);
    f32 z = (f32)cos(theta);

    vec3 rd = vec3(x, y, z);
    if(dot(n, rd) < 0.0f)
        rd = rd * -1.0f;
    return rd;
}

vec3
CosineWeightedSampleHemisphere(vec3 n) {
    vec2 pDisk = UniformSampleDisk();
    f32 z = (f32)sqrt(max(0.0f, 1.0f - (pDisk.x * pDisk.x) - (pDisk.y * pDisk.y)));
    return vec3(pDisk.x, pDisk.y, z);
}

CollisionInfo
CollisionRoutine(Scene *scene, vec3 ro, vec3 rd) {
    CollisionInfo result;
    
    result.t = FLT_MAX;

    f32 epsilon = 0.001f;
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
                        if((t1 > epsilon) || (t2 > epsilon)) {
                            f32 t_temp = pos_min(t1, t2);
                            if(t_temp < result.t) {
                                result.t = t_temp;
                                result.p = ro + result.t*rd;               
                                result.n = normalize(result.p - currentEntity.offset);
                                result.entityMat = currentEntity.shape.material;
                                result.entityIndex = entityIndex;
                            }
                        }
                    }
                                
                } break;

                case PLANE: {
                    f32 t_temp = dot((currentEntity.shape.pPlane - ro), currentEntity.shape.nPlane) /
                        dot(rd, currentEntity.shape.nPlane);
                    if((t_temp > epsilon) && (t_temp < result.t) && (dot(currentEntity.shape.nPlane, -rd) > 0.0f)) {
                        result.t = t_temp;
                        result.p = ro + result.t*rd;
                        result.n = currentEntity.shape.nPlane;
                        result.entityMat = currentEntity.shape.material;
                        result.entityIndex = entityIndex;                              
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
                    if((t_temp > epsilon) && (t_temp < result.t) && (dot(n, rd) > 0.0f)) {
                        result.t = t_temp;
                        result.p = intersection;
                        result.n = n;
                        result.entityMat = currentEntity.model.materials[GetMaterialIndex(currentEntity.model, baseVertex)];
                        result.entityIndex = entityIndex;                              
                    }
                }
            }
        }
    }

    return result;
}

f32
ApproximateUnoccludedArea(Scene *scene, u32 objectIndex, vec3 p, u32 nSamples) {
    if(nSamples == 0) 
        return 0;
    
    Entity object = scene->entities[objectIndex];
    f32 dist = magnitude(object.offset - p);
    vec3 rayToObject = normalize(object.offset - p);    
    f32 coneAngle = (f32)atan(object.shape.radius / dist);

    CollisionInfo collision = {};
    u32 numberOfHits = 0;
    
    for(u32 sampleIndex = 0; sampleIndex < nSamples; sampleIndex++) {
        vec3 samplePoint = CosineWeightedSampleHemisphere(-rayToObject);
        samplePoint += scene->entities[objectIndex].offset;
        vec3 sampleRay = normalize(samplePoint - p);
        collision = CollisionRoutine(scene, p, sampleRay);
        if((collision.t != FLT_MAX) && (collision.entityIndex == objectIndex))
            numberOfHits++;
    }
    
    return (f32)numberOfHits / (f32)nSamples;
}


vec3
TracePath(Scene *scene, vec3 ro, vec3 rd, u32 pathDepth) {
    CollisionInfo collision = {};
    f32 NEE_Attenuation = 1.0f;

    collision = CollisionRoutine(scene, ro, rd);
    Entity *entity = &scene->entities[collision.entityIndex];
    
    if(collision.t == FLT_MAX)
        return vec3(0.0f, 0.0f, 0.0f);
    if(entity->isEmitter)
        return entity->emission.flux * entity->emission.color;
    if(pathDepth == 0)
        return vec3(0.0f, 0.0f, 0.0f);
                
    //rendering function
    u32 emitterCount = 0;
    /*
    vec3 directLight = vec3(0.0f, 0.0f, 0.0f);
    for(u32 entityIndex = 0; entityIndex < (u32)scene->entities.size; entityIndex++) {
        if(scene->entities[entityIndex].isEmitter) {
            Entity *emitter = &scene->entities[entityIndex];
                        
            vec3 BRDF = collision.entityMat.diffuse / (f32)PI;
            vec3 rayToEmitterCenter = normalize(emitter->offset - collision.p);
            f32 cos_theta = max(0.0f, dot(collision.n, rayToEmitterCenter));

            f32 distToEmitter = dist(emitter->offset, collision.p);
            f32 attenuation = 1.0f / max(1.0f, (distToEmitter * distToEmitter));
            vec3 incomingRadiance = emitter->emission.flux * emitter->emission.color * attenuation;

            if(entityIndex != collision.entityIndex) {
                u32 nSamples = 0;
                f32 percentUnoccluded = ApproximateUnoccludedArea(scene, entityIndex, collision.p, nSamples);
                directLight *= percentUnoccluded;
            }
    
            vec3 emittedRadiance = vec3(0.0f, 0.0f, 0.0f);
            if(scene->entities[collision.entityIndex].isEmitter) {
                emittedRadiance = scene->entities[collision.entityIndex].emission.flux * scene->entities[collision.entityIndex].emission.color;
            }
            directLight += (emittedRadiance + (BRDF * incomingRadiance * cos_theta));
            emitterCount++;
        }
    }
    directLight /= (f32)emitterCount;
    */
    vec3 BRDF = collision.entityMat.diffuse / (f32)PI;
    f32 cosTheta = max(0.0f, dot(collision.n, -rd));
    
    ro = collision.p;
    rd = UniformSampleHemisphere(collision.n);

    return BRDF * TracePath(scene, ro, rd, pathDepth - 1) * cosTheta;
}

void
Draw(Camera *camera, Scene *scene) {
    int pathDepth = 16;
    
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

            vec3 finalColor = TracePath(scene, ro, rd, pathDepth);

            finalColor /= 1.0f + magnitude(finalColor);     
            finalColor = vec3((f32)pow((double)finalColor.x, (double)(1.0/2.2)), //gamma correction
                              (f32)pow((double)finalColor.y, (double)(1.0/2.2)),
                              (f32)pow((double)finalColor.z, (double)(1.0/2.2)));
//            finalColor = vec3((f32)sqrt(finalColor.x), (f32)sqrt(finalColor.y), (f32)sqrt(finalColor.z));
            
            u32 color = get_u32_color(finalColor * scene->skyColor);
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


