#ifndef I_SIEVE
#define I_SIEVE

#include <vector>

class ISieve 
{
public:
    virtual ~ISieve() = default;

    virtual void setMaxLimit(unsigned int maxLimit) = 0;
    virtual void computePrimes() = 0;
    virtual const std::vector<unsigned int>& getPrimes() const = 0;
    virtual void collectPrimes() = 0;
};

#endif  // I_SIEVE