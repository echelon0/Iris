
#define u8  unsigned char
#define u32 unsigned int
#define f32 float

#include <sys/stat.h>
#include "RNG.h"

#include <float.h>
#include "string.cpp"
#include "array.cpp"
#include "math.h"
#include "renderer.cpp"
#include "file_loader.cpp"

bool
InitIris() {
    if(!InitRNG())
        return false;
    return true;
}

void
FreeIris() {
    FreeRNG();
}
