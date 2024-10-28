#include "SingleCpuSieve.hpp"
#include "iostream"

SingleCpuSieve::SingleCpuSieve(unsigned int maxLimit)
	: maxLimit_(maxLimit)
	, primes_()
{
	isPrime_.reserve(maxLimit_ + 1);
}

void SingleCpuSieve::computePrimes()
{
	isPrime_.assign(maxLimit_ + 1, true);
	for (unsigned int number = 2; number * number <= maxLimit_; ++number) 
	{
		if (isPrime_[number])
		{
			for (size_t multiply = number * number; multiply <= maxLimit_; multiply += number)
			{
				isPrime_[multiply] = false;
			}
		}
	}
}

void SingleCpuSieve::collectPrimes()
{
	for (unsigned int number = 2; number <= maxLimit_; ++number)
	{
		if (isPrime_[number]) primes_.push_back(number);
	}
}

void SingleCpuSieve::setMaxLimit(unsigned int maxLimit)
{
	maxLimit_ = maxLimit;
	isPrime_.assign(maxLimit_ + 1, true);
	primes_.clear();
}

const std::vector<unsigned int>& SingleCpuSieve::getPrimes() const
{
	return primes_;
}