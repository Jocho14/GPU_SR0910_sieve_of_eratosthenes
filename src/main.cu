#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
#include <chrono>
#include <cuda_runtime.h>

#include "iostream"
#include "memory"

#include "common/timer/Timer.hpp"

#include "cpu/singleThreaded/SingleCpuSieve.hpp"
#include "cpu/multiThreaded/MultiCpuSieve.hpp"
#include "gpu/GpuSieve.hpp"

int main() {
    auto timer = std::make_shared<Timer>();

    // Test dla SingleCpuSieve
    auto single = std::make_shared<SingleCpuSieve>(1000000);
    timer->start();
    single->computePrimes();
    timer->stop();
    std::cout << "Time taken to compute primes on single thread: " << timer->getTime() << "ms" << std::endl;

    timer->reset();

    // Test dla MultiCpuSieve
    auto multi = std::make_shared<MultiCpuSieve>(1000000);
    timer->start();
    multi->computePrimes();
    timer->stop();
    std::cout << "Time taken to compute primes on multi threads: " << timer->getTime() << "ms" << std::endl;

    timer->reset();

    // Test dla GpuSieve
    auto gpuSieve = std::make_shared<GpuSieve>();
    gpuSieve->setMaxLimit(1000000);
    timer->start();
    gpuSieve->computePrimes();
    timer->stop();
    std::cout << "Time taken to compute primes on GPU: " << timer->getTime() << "ms" << std::endl;

    // Zbieranie wyników
    single->collectPrimes();
    multi->collectPrimes();
    gpuSieve->collectPrimes();

    // Wyświetlanie liczby liczb pierwszych
    std::cout << "NUM OF PRIMES FOR SINGLE: " << single->getPrimes().size() << std::endl;
    std::cout << "NUM OF PRIMES FOR MULTI: " << multi->getPrimes().size() << std::endl;
    std::cout << "NUM OF PRIMES FOR GPU: " << gpuSieve->getPrimes().size() << std::endl;

    // Porównanie wyników między implementacjami
    if (single->getPrimes().size() == multi->getPrimes().size() &&
        multi->getPrimes().size() == gpuSieve->getPrimes().size()) {
        std::cout << "All implementations produced the same number of primes." << std::endl;
    }
    else {
        std::cout << "Mismatch in the number of primes found by different implementations." << std::endl;
    }

    return 0;
}