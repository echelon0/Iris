

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

bool
Sample(Camera camera, Scene scene) {
    vec3 sphere_center = vec3(0.0, 0.0, 10.0);
    float sphere_radius = 3.0f;

    int bounceCount = 5;
    
    u32 *pixel = (u32 *)camera.film.buffer;
    for(int y = 0; y < camera.film.pixelHeight; y++) {    
        for(int x = 0; x < camera.film.pixelWidth; x++) {
            float xDist = ((x / (f32)camera.film.pixelWidth) - 0.5f) * camera.film.worldSize.x;
            float yDist = ((y / (f32)camera.film.pixelHeight) - 0.5f) * camera.film.worldSize.y;
            vec3 pFilm = camera.pos + (camera.dir * camera.film.dist) + (camera.right * xDist) + (camera.up * yDist);
            vec3 ro = camera.pos;
            vec3 rd = normalize(pFilm - camera.pos);

            int r = 0;
            int g = 0;
            int b = 0;            
            for(int i = 0; i < bounceCount; i++) {

                bool hitLight = false;
                float a = dot(rd, rd);
                float b = 2.0f * dot(rd, ro - sphere_center);
                float c = dot((ro - sphere_center), (ro - sphere_center)) - (sphere_radius * sphere_radius);

                float discriminant = b*b - 4.0f*a*c;
                if(discriminant >= 0) {
                    float t1 = (-b - (f32)sqrt(discriminant)) / 2.0f * a;
                    float t2 = (-b + (f32)sqrt(discriminant)) / 2.0f * a;
                    float t = min(t1, t2);                  
                    if(t1 < 0) {
                        t = t2;
                    } else if(t2 < 0) {
                        t = t1;
                    }

                    vec3 pCollision = ro + t*rd;
                    vec3 n = normalize(pCollision - sphere_center);
                    ro = pCollision;
                    rd = normalize(rd + n * 2.0f * dot(-rd, n));
                    
                    if(t > 0.0000001f) {
                        r += 0;
                        g += 100;
                        b += 0;
                    } else {
                        hitLight = true;
                    }
                }
                if(hitLight || discriminant < 0) {
                    r += 100;
                    g += 0;
                    b += 0;
                    break;
                }
            }
            *pixel++ = (0 << 24) | (r << 16) | (g << 8) | (b << 0);
        }
    }
    return true;
}

