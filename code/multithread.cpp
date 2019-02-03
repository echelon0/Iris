
#include "multithread.h"

void
EnableMultithreading() {
//    Multithreading.MaxThreads = std::thread::hardware_concurrency() * 2;
    if(Multithreading.MaxThreads > 0) {
//        Multithreading.Threads = (std::thread *)malloc(Multithreading.MaxThreads * sizeof(std::thread));
        Multithreading.Enabled = true;
    }
}

