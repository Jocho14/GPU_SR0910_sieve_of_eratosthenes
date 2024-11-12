#include <cmath>
#include <cuda_runtime.h>
#include "GpuSieve.hpp"
#include <iostream>
#include <vector>
#include <cassert>

// Maximum stride for GPU processing
const uint64_t MAX_STRIDE = 256000000;
// Number of threads per block (adjustable based on GPU)
int BLOCK_SIZE = 256;

// Constructor: Initialize GpuSieve object with no allocated memory on the host
GpuSieve::GpuSieve() : maxLimit(0), isPrimeListHost(nullptr) {}

// Destructor: Free host memory if allocated
GpuSieve::~GpuSieve() {
    // Free host memory
    if (isPrimeListHost) {
        free(isPrimeListHost);
        isPrimeListHost = nullptr;
    }
}

// Set the maximum limit for prime computation
void GpuSieve::setMaxLimit(unsigned int maxLimit) {
    this->maxLimit = maxLimit;
}

// Main function to compute primes up to maxLimit
void GpuSieve::computePrimes() {
    // Prepare initial list of small primes on CPU using a simple sieve
    std::vector<uint64_t> prepedPrimes = sieveCpuPrep(std::sqrt(maxLimit));

    // Launch GPU sieve to find all primes up to maxLimit
    gpuSieve(maxLimit, prepedPrimes);
}

// Retrieve the list of primes found
const std::vector<unsigned int>& GpuSieve::getPrimes() const {
    return primes;
}

// Collect primes from the result list on the host
void GpuSieve::collectPrimes() {
    // Clear any existing primes in the list
    primes.clear();
    // Traverse isPrimeListHost to collect primes up to maxLimit
    for (uint64_t i = 2; i <= maxLimit; i++) {
        if (isPrimeListHost[i]) {
            primes.push_back(i);
        }
    }
}

// Prepare a list of initial small primes on the CPU (sieve of Eratosthenes)
std::vector<uint64_t> GpuSieve::sieveCpuPrep(uint64_t maxNumber) {
    std::vector<bool> isPrimeList(maxNumber, true);
    std::vector<uint64_t> prepedPrimes;

    // Basic sieve algorithm to mark non-primes
    for (uint64_t i = 2; i < maxNumber; i++) {
        if (isPrimeList[i]) {
            prepedPrimes.push_back(i);
            // Mark multiples of i as non-prime
            for (uint64_t j = 2; i * j < maxNumber; j++) {
                isPrimeList[i * j] = false;
            }
        }
    }
    return prepedPrimes;
}

// CUDA kernel to perform the sieve on the GPU
__global__ void gpuSieveKernel(uint64_t maxNumber, bool* isPrimeList, uint64_t* prepedPrimes, uint64_t sizeOfPrepedPrimes) {
    uint64_t threadIndex = blockIdx.x * blockDim.x + threadIdx.x;
    uint64_t stride = blockDim.x * gridDim.x;

    // Iterate over each position in the range for this thread
    for (uint64_t i = threadIndex; i <= maxNumber; i += stride) {
        if (i < 2) continue;
        // Mark multiples of precomputed primes as non-prime
        for (uint64_t j = 0; j < sizeOfPrepedPrimes; j++) {
            uint64_t currentNumber = prepedPrimes[j] * i;
            if (currentNumber > maxNumber) break;
            isPrimeList[currentNumber] = false;
        }
    }
}

// Function to launch GPU sieve with prepared primes
void GpuSieve::gpuSieve(uint64_t maxNumber, const std::vector<uint64_t>& prepedPrimes) {
    bool* isPrimeListDevice = nullptr;
    uint64_t* prepedPrimesDevice = nullptr;

    // Allocate and initialize memory on host and device
    isPrimeListHost = (bool*)malloc(maxNumber * sizeof(bool));
    std::memset(isPrimeListHost, true, maxNumber * sizeof(bool));
    cudaMalloc(&isPrimeListDevice, maxNumber * sizeof(bool));
    cudaMemcpy(isPrimeListDevice, isPrimeListHost, maxNumber * sizeof(bool), cudaMemcpyHostToDevice);

    // Prepare and copy the list of initial primes to device
    uint64_t prepedPrimesSize = prepedPrimes.size();
    uint64_t* prepedPrimesHost = (uint64_t*)malloc(prepedPrimesSize * sizeof(uint64_t));
    memcpy(prepedPrimesHost, prepedPrimes.data(), prepedPrimesSize * sizeof(uint64_t));
    cudaMalloc(&prepedPrimesDevice, prepedPrimesSize * sizeof(uint64_t));
    cudaMemcpy(prepedPrimesDevice, prepedPrimesHost, prepedPrimesSize * sizeof(uint64_t), cudaMemcpyHostToDevice);

    // Configure blocks and threads for kernel launch
    uint64_t numberOfBlocks = (maxNumber + BLOCK_SIZE - 1) / BLOCK_SIZE;
    if (numberOfBlocks * BLOCK_SIZE > MAX_STRIDE) {
        numberOfBlocks = MAX_STRIDE / BLOCK_SIZE;
    }

    // Launch the kernel
    gpuSieveKernel << <numberOfBlocks, BLOCK_SIZE >> > (maxNumber, isPrimeListDevice, prepedPrimesDevice, prepedPrimesSize);
    cudaDeviceSynchronize();

    // Copy results back to the host
    cudaMemcpy(isPrimeListHost, isPrimeListDevice, maxNumber * sizeof(bool), cudaMemcpyDeviceToHost);

    // Clean up memory
    free(prepedPrimesHost);
    cudaFree(isPrimeListDevice);
    cudaFree(prepedPrimesDevice);
}
