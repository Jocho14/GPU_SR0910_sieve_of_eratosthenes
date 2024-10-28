#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include "../../interfaces/ISieve.hpp"

class MultiCpuSieve : public ISieve 
{
public:
    MultiCpuSieve(unsigned int maxLimit, unsigned int numThreads = std::thread::hardware_concurrency());

    void computePrimes() override;
    void collectPrimes() override;
    void setMaxLimit(unsigned int maxLimit) override;
    const std::vector<unsigned int>& getPrimes() const override;

private:
    unsigned int maxLimit_;
    unsigned int numThreads_;
    std::vector<bool> isPrime_;
    std::vector<unsigned int> primes_;
    std::mutex primesMutex_;

    void markMultiplies(unsigned int prime, unsigned int start, unsigned int end);
};
