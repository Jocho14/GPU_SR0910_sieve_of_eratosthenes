#ifndef SINGLE_CPU_SIEVE
#define SINGLE_CPU_SIEVE

#include "vector"

#include "../../interfaces/ISieve.hpp"

class SingleCpuSieve : public ISieve 
{
public:
    SingleCpuSieve(unsigned int maxLimit);

    void computePrimes() override;
    void collectPrimes() override;
    void setMaxLimit(unsigned int maxLimit) override;
    const std::vector<unsigned int>& getPrimes() const override;

private:
    unsigned int maxLimit_;
    std::vector<bool> isPrime_;
    std::vector<unsigned int> primes_;
};

#endif  // SINGLE_CPU_SIEVE
