#include "iostream"
#include "memory"

#include "common/timer/Timer.hpp"

#include "cpu/singleThreaded/SingleCpuSieve.hpp"
#include "cpu/multiThreaded/MultiCpuSieve.hpp"


int main()
{
	auto timer = std::make_shared<Timer>();

	auto single = std::make_shared<SingleCpuSieve>(1000000000);
	auto multi = std::make_shared<MultiCpuSieve>(1000000000);
	
	timer->start();
	single->computePrimes();
	timer->stop();
	std::cout << "Time taken to compute primes on single thread: " << timer->getTime() << "ms" << std::endl;

	timer->reset();

	timer->start();
	multi->computePrimes();
	timer->stop();
	std::cout << "Time taken to compute primes on multi threads: " << timer->getTime() << "ms";

	timer->reset();

	single->collectPrimes();
	multi->collectPrimes();

	std::cout << "NUM OF PRIMES FOR SINGLE: " << single->getPrimes().size() << std::endl;
	std::cout << "NUM OF PRIMES FOR MULTI: " << multi->getPrimes().size();

	return 0;
}
