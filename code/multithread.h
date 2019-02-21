
#include <thread>

struct iris_multithread {
    bool Enabled = false;
    u32 MaxThreads = 3;
    std::thread Threads[3];    
} Multithreading;
