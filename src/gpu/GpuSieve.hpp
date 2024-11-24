#ifndef GPU_SIEVE_HPP
#define GPU_SIEVE_HPP

#include "../interfaces/ISieve.hpp"
#include <vector>
#include <cstdint>
#include <memory>


class GpuSieve : public ISieve {
public:
    GpuSieve();
    ~GpuSieve() override;

    void setMaxLimit(unsigned int maxLimit) override;
    void computePrimes() override;
    const std::vector<unsigned int>& getPrimes() const override;
    void collectPrimes() override;

private:
    unsigned int maxLimit_;
    std::unique_ptr isPrimeListHost_;
    std::vector<unsigned int> primes_;

    std::vector<uint64_t> sieveCpuPrep_(uint64_t maxNumber);
    void gpuSieve_(uint64_t maxNumber, const std::vector<uint64_t>& prepedPrimes);
};

#endif  // GPU_SIEVE_HPP
