
#include "renderer.h"
#include "camera.cpp"

int
GetMaterialIndex(model Model, int VertexIndex) {
    for(int BaseIndex = 0; BaseIndex < Model.MaterialBases.Size; BaseIndex++) {
        if(VertexIndex >= Model.MaterialBases[BaseIndex] &&
           (VertexIndex < Model.MaterialBases[BaseIndex] + Model.MaterialSizes[BaseIndex])) {
            return BaseIndex;
        }
    };
    return -1;
}

void
RenderBuffer(HDC deviceContext, void *buffer,
             int WindowWidth, int WindowHeight,
             int bufferWidth, int bufferHeight) {
    
    BITMAPINFO bitmapInfo = {};
    bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
    bitmapInfo.bmiHeader.biWidth = bufferWidth;
    bitmapInfo.bmiHeader.biHeight = bufferHeight;
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 32;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;
    
    StretchDIBits(deviceContext,
                  0, 0, WindowWidth, WindowHeight,
                  0, 0, bufferWidth, bufferHeight,
                  buffer,
                  &bitmapInfo,
                  DIB_RGB_COLORS,
                  SRCCOPY);
}

inline u32
Get_u32Color(vec3 vec) {
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

collision_info
CollisionRoutine(scene *Scene, vec3 ro, vec3 rd) {
    collision_info Result = {};
    
    Result.t = FLT_MAX;

    f32 Epsilon = 0.001f;
    for(int EntityIndex = 0; EntityIndex < Scene->Entities.Size; EntityIndex++) {
        entity CurrentEntity = Scene->Entities[EntityIndex];
        if(CurrentEntity.IsShape) { //--- Shape ---
            switch(CurrentEntity.Shape.Type) {
                case SPHERE: {
                    f32 a = dot(rd, rd);
                    f32 b = 2.0f * dot(rd, ro - CurrentEntity.Offset);
                    f32 c = dot((ro - CurrentEntity.Offset), (ro - CurrentEntity.Offset)) - (CurrentEntity.Shape.Radius * CurrentEntity.Shape.Radius);

                    f32 Discriminant = b*b - 4.0f*a*c;
                    if(Discriminant >= 0) {
                        f32 t1 = (-b - (f32)sqrt(Discriminant)) / 2.0f * a;
                        f32 t2 = (-b + (f32)sqrt(Discriminant)) / 2.0f * a;
                        if((t1 > Epsilon) || (t2 > Epsilon)) {
                            f32 t_temp = pos_min(t1, t2);
                            if(t_temp < Result.t) {
                                Result.t = t_temp;
                                Result.p = ro + Result.t*rd;               
                                Result.n = normalize(Result.p - CurrentEntity.Offset);
                                Result.EntityMat = CurrentEntity.Shape.Material;
                                Result.EntityIndex = EntityIndex;
                            }
                        }
                    }
                                
                } break;

                case PLANE: {
                    f32 t_temp = dot(CurrentEntity.Shape.pPlane - ro, CurrentEntity.Shape.nPlane) / dot(rd, CurrentEntity.Shape.nPlane);
                    if((t_temp > Epsilon) && (t_temp < Result.t)) {
                        Result.t = t_temp;
                        Result.p = ro + Result.t*rd;
                        Result.n = CurrentEntity.Shape.nPlane;
                        Result.EntityMat = CurrentEntity.Shape.Material;
                        Result.EntityIndex = EntityIndex;                              
                    }                               
                                
                } break;
                                
                default: {
                } break;
            }
        } else { //--- Model ---
            for(int BaseVertex = 0; BaseVertex < CurrentEntity.Model.VertexAttributes.Size; BaseVertex += 3) {
                vec3 n = CurrentEntity.Model.VertexAttributes[BaseVertex].Normal;                          
                vec3 v0 = CurrentEntity.Model.VertexAttributes[BaseVertex + 0].Position + CurrentEntity.Offset;
                vec3 v1 = CurrentEntity.Model.VertexAttributes[BaseVertex + 1].Position + CurrentEntity.Offset;
                vec3 v2 = CurrentEntity.Model.VertexAttributes[BaseVertex + 2].Position + CurrentEntity.Offset;

                f32 t_temp;
                if(ray_intersects_triangle(ro, rd, v0, v1, v2, n, t_temp)) {
                    if((t_temp > Epsilon) && (t_temp < Result.t)) {
                        Result.t = t_temp;
                        Result.p = ro + Result.t*rd;
                        Result.n = n;
                        Result.EntityMat = CurrentEntity.Model.Materials[GetMaterialIndex(CurrentEntity.Model, BaseVertex)];
                        Result.EntityIndex = EntityIndex;                              
                    }
                }
            }
        }
    }

    return Result;
}

vec3
SampleDirectLight(scene *Scene, vec3 p, u32 SampleCount) {
    if(SampleCount < 1) 
        return vec3(0.0f, 0.0f, 0.0f);

    vec3 DirectLight = vec3(0.0f, 0.0f, 0.0f);
    u32 LightCount = 0;
    for(u32 EntityIndex = 0; EntityIndex < (u32)Scene->Entities.Size; EntityIndex++) {
        f32 DistToLight = 0.0f;
        vec3 LightRay = vec3(0.0f, 0.0f, 0.0f);
        f32 Attenuation = 0.0f;
        f32 Visibility = 0.0f;
        
        if(Scene->Entities[EntityIndex].IsEmitter) {
            entity *Light = &Scene->Entities[EntityIndex];
            if(Light->IsShape) { //--- Shape Light ---
                switch(Light->Shape.Type) {
                    case SPHERE: {
                        DistToLight = magnitude(Light->Offset - p);
                        LightRay = normalize(Light->Offset - p);
                        Attenuation = 1.0f / max(1.0f, DistToLight * DistToLight);
                        f32 Theta = (f32)atan(Light->Shape.Radius / DistToLight);

                        Visibility = 0.0f;
                        for(u32 SampleIndex = 0; SampleIndex < SampleCount; SampleIndex++) {
                            f32 R1 = absf((f32)cos((f32)g_RNG.rand_u32()));
                            f32 R2 = absf((f32)cos((f32)g_RNG.rand_u32()));
                            f32 SolidAngle = 2.0f * (f32)PI * (1.0f - (f32)cos(Theta));                     
                            f32 qSolidAngle = (1.0f - R2 * SolidAngle);

                            f32 X = (f32)cos(2.0f * (f32)PI * R1) * (f32)sqrt(1.0f - R2*R2);
                            f32 Y = (f32)sin(2.0f * (f32)PI * R1) * (f32)sqrt(1.0f - R2*R2);
                            f32 Z = R2;
                            
                            vec3 Sample = vec3(X, Y, Z);
                            vec3 Offset = Sample - vec3(0.0f, 0.0f, 1.0f);
                            Sample = LightRay + Offset;
                            
                            vec3 ro = p;                    
                            vec3 rd = Sample;
                            collision_info Collision = CollisionRoutine(Scene, ro, rd);
                            if(Collision.t < FLT_MAX && Collision.EntityIndex == EntityIndex) {
                                Visibility += 1.0f;
                            }
                        }
                        Visibility /= (f32)SampleCount;

                    } break;
                        
                    default: {
                    } break;
                }
            } else { //--- Model Light ---
                //TODO
            }

            DirectLight += Visibility * Attenuation * Light->Emission.Flux * Light->Emission.Color;         
        }       
    }

    LightCount = (LightCount > 0)? LightCount : 1; //prevents division by zero
    return DirectLight / (f32)LightCount;
}

vec3
TracePath(scene *Scene, vec3 ro, vec3 rd, u32 PathDepth, u32 nDirectSamples) {
    collision_info Collision = {};

    Collision = CollisionRoutine(Scene, ro, rd);
    entity Entity = Scene->Entities[Collision.EntityIndex];
    material EntityMat = Collision.EntityMat;

    vec3 DirectLight = vec3(0.0f, 0.0f, 0.0f);    
    if(Collision.t == FLT_MAX)
        return vec3(0.0f, 0.0f, 0.0f);
    if(PathDepth <= 1)
        return vec3(0.0f, 0.0f, 0.0f);
                
    vec3 BRDF = EntityMat.Diffuse / (f32)PI;
    f32 cosTheta = dot(Collision.n, -rd);
    if(Entity.IsEmitter)
        DirectLight += Entity.Emission.Flux * Entity.Emission.Color;
    else
        DirectLight += SampleDirectLight(Scene, Collision.p, nDirectSamples);
    
    ro = Collision.p;
    rd = UniformSampleHemisphere(Collision.n);
        
    vec3 OutgoingRadiance = BRDF * TracePath(Scene, ro, rd, PathDepth - 1, nDirectSamples) * absf(cosTheta) + DirectLight;
    return OutgoingRadiance;
}

void
DrawRect(camera *Camera, scene *Scene,
         u32 PathDepth, u32 nDirectSamples,
         int xStart, int yStart,
         int xEnd, int yEnd) {
    
    for(int y = yStart; y < yEnd; y++) {    
        for(int x = xStart; x < xEnd; x++) {
            f32 xDist = ((x / (f32)Camera->Film.PixelWidth) - 0.5f) * Camera->Film.WorldSize.x;
            f32 yDist = ((y / (f32)Camera->Film.PixelHeight) - 0.5f) * Camera->Film.WorldSize.y;
            vec3 pFilm = Camera->Pos + (Camera->Dir * Camera->Film.Dist) + (Camera->Right * xDist) + (Camera->Up * yDist);
            vec3 ro = Camera->Pos;
            vec3 rd = normalize(pFilm - Camera->Pos);

            vec3 finalColor = TracePath(Scene, ro, rd, PathDepth, nDirectSamples);
            
            finalColor /= 1.0f + magnitude(finalColor);
            finalColor = vec3((f32)pow((double)finalColor.x, (double)(1.0/2.2)), //gamma correction
                              (f32)pow((double)finalColor.y, (double)(1.0/2.2)),
                              (f32)pow((double)finalColor.z, (double)(1.0/2.2)));
            
            u32 Color = Get_u32Color(finalColor);
            u32 FilmIndex = (y * Camera->Film.PixelWidth) + x;     
            ((u32 *)Camera->Film.SumBuffer)[(FilmIndex * 4) + 0] += (Color >> 0) & 0xFF;
            ((u32 *)Camera->Film.SumBuffer)[(FilmIndex * 4) + 1] += (Color >> 8) & 0xFF;
            ((u32 *)Camera->Film.SumBuffer)[(FilmIndex * 4) + 2] += (Color >> 16) & 0xFF;
            ((u32 *)Camera->Film.SumBuffer)[(FilmIndex * 4) + 3] += (Color >> 24) & 0xFF;
        }
    }
}

void
Draw(camera *Camera, scene *Scene, u32 PathDepth, u32 nDirectSamples) {
    if(Multithreading.Enabled) {
        const u32 nRects = 32;
        int RectHeight = Camera->Film.PixelHeight / nRects;
        int Leftover = Camera->Film.PixelHeight % nRects;
        u32 WorkMap[nRects];
        memset(WorkMap, 0, sizeof(u32) * nRects);
        u32 RectsDrawn = 0;
        while(RectsDrawn < nRects) {
            for(u32 ThreadIndex = 0; ThreadIndex < Multithreading.MaxThreads; ThreadIndex++) {
                if(!Multithreading.Threads[ThreadIndex].joinable()) {
                    for(u32 MapIndex = 0; MapIndex < nRects; MapIndex++) {
                        if(!WorkMap[MapIndex]) {
                            int xStart = 0;
                            int yStart = MapIndex * RectHeight;
                            int xEnd = Camera->Film.PixelWidth;
                            int yEnd = yStart + RectHeight;
                            Multithreading.Threads[ThreadIndex] = std::thread(DrawRect, Camera, Scene, PathDepth, nDirectSamples, xStart, yStart, xEnd, yEnd);
                            WorkMap[MapIndex] = 1;
                            RectsDrawn++;

                            break;
                        }
                    }
                }
            }
            for(u32 ThreadIndex = 0; ThreadIndex < Multithreading.MaxThreads; ThreadIndex++) {
                if(Multithreading.Threads[ThreadIndex].joinable())
                    Multithreading.Threads[ThreadIndex].join();
            }
        }
        
    } else {
        DrawRect(Camera, Scene, PathDepth, nDirectSamples, 0, 0, Camera->Film.PixelWidth, Camera->Film.PixelHeight);
    }

    Camera->SampleCount++;
    u32 TotalPixelChannels = Camera->Film.PixelWidth * Camera->Film.PixelHeight * 4;
    for(u32 i = 0; i < TotalPixelChannels; i++) {
        ((u8 *)Camera->Film.Buffer)[i] = (u8) ((f32)(((u32 *)Camera->Film.SumBuffer)[i]) / (f32)Camera->SampleCount);
    }
}


