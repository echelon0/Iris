
#include <thread>

struct iris_multithread {
    bool Enabled = false;
    u32 MaxThreads = 15;
    std::thread Threads[15];    
} Multithreading;
