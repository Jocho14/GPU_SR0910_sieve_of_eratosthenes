#include "MultiCpuSieve.hpp"
#include <cmath>
#include <algorithm>

MultiCpuSieve::MultiCpuSieve(unsigned int maxLimit, unsigned int numThreads)
    : maxLimit_(maxLimit)
    , numThreads_(numThreads)
    , primes_()
{
    isPrime_.assign(maxLimit_ + 1, true);
}

void MultiCpuSieve::setMaxLimit(unsigned int maxLimit)
{
    maxLimit_ = maxLimit;
    isPrime_.assign(maxLimit_ + 1, true);
    primes_.clear();
}

void MultiCpuSieve::computePrimes() {
    unsigned int sqrtLimit = static_cast<unsigned int>(std::sqrt(maxLimit_));

    for (unsigned int number = 2; number * number <= maxLimit_; ++number) 
    {
        if (isPrime_[number])
        {
            std::vector<std::thread> threads;
            unsigned int chunkSize = (maxLimit_ - number * number + 1) / numThreads_;

            for (unsigned int t = 0; t < numThreads_; ++t) 
            {
                unsigned int start = number * number + t * chunkSize;
                unsigned int end = std::min(start + chunkSize, maxLimit_ + 1);

                if (start % number != 0)
                {
                    start += number - (start % number);
                }

                threads.emplace_back(&MultiCpuSieve::markMultiplies, this, number, start, end);
            }

            for (auto& thread : threads) 
            {
                if (thread.joinable()) 
                {
                    thread.join();
                }
            }
        }
    }
}

void MultiCpuSieve::markMultiplies(unsigned int prime, unsigned int start, unsigned int end)
{
    for (unsigned int number = start; number < end; number += prime)
    {
        isPrime_[number] = false;
    }
}

const std::vector<unsigned int>& MultiCpuSieve::getPrimes() const 
{
    return primes_;
}

void MultiCpuSieve::collectPrimes() 
{
    primes_.clear();
    for (unsigned int number = 2; number <= maxLimit_; ++number)
    {
        if (isPrime_[number])
        {
            primes_.push_back(number);
        }
    }
}
