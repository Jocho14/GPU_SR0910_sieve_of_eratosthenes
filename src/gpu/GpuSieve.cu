#include <cmath>
#include <cuda_runtime.h>
#include "GpuSieve.hpp"
#include <iostream>
#include <vector>
#include <cassert>

// Sta³e i zmienne globalne dla GPU
const uint64_t MAX_STRIDE = 256000000;
int BLOCK_SIZE = 256;

GpuSieve::GpuSieve() : maxLimit(0), isPrimeListHost(nullptr) {}

GpuSieve::~GpuSieve() {
    // Zwolnienie pamiêci hosta
    if (isPrimeListHost) {
        free(isPrimeListHost);
        isPrimeListHost = nullptr;
    }
}

void GpuSieve::setMaxLimit(unsigned int maxLimit) {
    this->maxLimit = maxLimit;
}

void GpuSieve::computePrimes() {
    // Przygotowanie wstêpnych liczb pierwszych na CPU
    std::vector<uint64_t> prepedPrimes = sieveCpuPrep(std::sqrt(maxLimit));

    // Wywo³anie sita na GPU
    gpuSieve(maxLimit, prepedPrimes);
}

const std::vector<unsigned int>& GpuSieve::getPrimes() const {
    return primes;
}

void GpuSieve::collectPrimes() {
    // Zbieranie liczb pierwszych z listy wyników na hoœcie
    primes.clear();
    for (uint64_t i = 2; i <= maxLimit; i++) {
        if (isPrimeListHost[i]) {
            primes.push_back(i);
        }
    }
}

std::vector<uint64_t> GpuSieve::sieveCpuPrep(uint64_t maxNumber) {
    std::vector<bool> isPrimeList(maxNumber, true);
    std::vector<uint64_t> prepedPrimes;

    for (uint64_t i = 2; i < maxNumber; i++) {
        if (isPrimeList[i]) {
            prepedPrimes.push_back(i);
            for (uint64_t j = 2; i * j < maxNumber; j++) {
                isPrimeList[i * j] = false;
            }
        }
    }
    return prepedPrimes;
}

__global__ void gpuSieveKernel(uint64_t maxNumber, bool* isPrimeList, uint64_t* prepedPrimes, uint64_t sizeOfPrepedPrimes) {
    uint64_t threadIndex = blockIdx.x * blockDim.x + threadIdx.x;
    uint64_t stride = blockDim.x * gridDim.x;

    for (uint64_t i = threadIndex; i <= maxNumber; i += stride) {
        if (i < 2) continue;
        for (uint64_t j = 0; j < sizeOfPrepedPrimes; j++) {
            uint64_t currentNumber = prepedPrimes[j] * i;
            if (currentNumber > maxNumber) break;
            isPrimeList[currentNumber] = false;
        }
    }
}

void GpuSieve::gpuSieve(uint64_t maxNumber, const std::vector<uint64_t>& prepedPrimes) {
    bool* isPrimeListDevice = nullptr;
    uint64_t* prepedPrimesDevice = nullptr;

    // Alokacja i inicjalizacja pamiêci na hoœcie i urz¹dzeniu
    isPrimeListHost = (bool*)malloc(maxNumber * sizeof(bool));
    std::memset(isPrimeListHost, true, maxNumber * sizeof(bool));
    cudaMalloc(&isPrimeListDevice, maxNumber * sizeof(bool));
    cudaMemcpy(isPrimeListDevice, isPrimeListHost, maxNumber * sizeof(bool), cudaMemcpyHostToDevice);

    uint64_t prepedPrimesSize = prepedPrimes.size();
    uint64_t* prepedPrimesHost = (uint64_t*)malloc(prepedPrimesSize * sizeof(uint64_t));
    memcpy(prepedPrimesHost, prepedPrimes.data(), prepedPrimesSize * sizeof(uint64_t));
    cudaMalloc(&prepedPrimesDevice, prepedPrimesSize * sizeof(uint64_t));
    cudaMemcpy(prepedPrimesDevice, prepedPrimesHost, prepedPrimesSize * sizeof(uint64_t), cudaMemcpyHostToDevice);

    // Konfiguracja bloków i w¹tków dla kernela
    uint64_t numberOfBlocks = (maxNumber + BLOCK_SIZE - 1) / BLOCK_SIZE;
    if (numberOfBlocks * BLOCK_SIZE > MAX_STRIDE) {
        numberOfBlocks = MAX_STRIDE / BLOCK_SIZE;
    }

    // Uruchomienie kernela
    gpuSieveKernel << <numberOfBlocks, BLOCK_SIZE >> > (maxNumber, isPrimeListDevice, prepedPrimesDevice, prepedPrimesSize);
    cudaDeviceSynchronize();

    // Kopiowanie wyników z urz¹dzenia na hosta
    cudaMemcpy(isPrimeListHost, isPrimeListDevice, maxNumber * sizeof(bool), cudaMemcpyDeviceToHost);

    // Czyszczenie pamiêci
    free(prepedPrimesHost);
    cudaFree(isPrimeListDevice);
    cudaFree(prepedPrimesDevice);
}
