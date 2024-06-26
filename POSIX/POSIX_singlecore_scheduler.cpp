#include <pthread.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include "timer.hpp"

/**
* Overload a single core.
* test NUMA arch & asymmetric (big.LITTLE) Cores design
* Code for Linux
* Ref: https://www.gnu.org/software/libc/manual/html_node/CPU-Affinity.html
*
* This code snippet suppose to execute a heavy computation on each core of your linux machine sequentially. Use htop to check core affinity.
*/

unsigned long long fibonacci_recursive(int n) {
    if (n <= 1) return n;
    return fibonacci_recursive(n - 1) + fibonacci_recursive(n - 2);
}

void* threadFunction(void* arg) {
    Timer t;
    unsigned long long  res = fibonacci_recursive(44);
    return (void*)res;
}

void threadlauncher(int cpunum){
    pthread_t thread;
    cpu_set_t cpuset;
    printf("Set CPU %d\n",cpunum);

    pthread_create(&thread, NULL, &threadFunction, NULL);

    CPU_ZERO(&cpuset);
    CPU_SET(cpunum, &cpuset);

    int result = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
    if (result != 0) {
        perror("Error");
    }

    pthread_join(thread, NULL);
}


int main() {
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << "Available Cores: " << n << std::endl;

    for(int i = 0; i<n; i++){
        threadlauncher(i);
    }

    return 0;
}
