
#include <thread>

struct iris_multithread {
    bool Enabled = false;
    u32 MaxThreads = 4;
    std::thread Threads[4];    
} Multithreading;
