
struct RNG {
    u32 r;
    u32 rand_u32() {
        r ^= r << 13;
        r ^= r >> 17;
        r ^= r << 5;
        return r;
    }
};

RNG g_RNG = {(u32)1745098127};

