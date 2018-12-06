
struct RNG {
    void *noise;
    u32 size;
    u32 index;

    u32 rand_u32() {
        if(index >= (size / sizeof(u32)))
            index = 0;
        u32 result = ((u32 *)noise)[index];
        index ++; //((result << 10) + (result << 4)); //increment index by random value
        return result;
    }
};

RNG g_RNG;

bool
InitRNG() {
    g_RNG = {};
    struct stat fileStat;
    stat("../iris/noise.png", &fileStat);
    g_RNG.noise = (void *)malloc(fileStat.st_size);

    FILE *fileHandle = fopen("../iris/noise.png", "rb");
    if(!fileHandle) {
        return false;
    }

    g_RNG.size = (u32)fread(g_RNG.noise, 1, fileStat.st_size, fileHandle);
    fclose(fileHandle);
    return true;
}

void
FreeRNG() {
    delete g_RNG.noise;
}
