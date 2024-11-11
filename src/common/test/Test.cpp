#include "test.hpp"


std::shared_ptr<Timer> Test::timer = std::make_shared<Timer>();

void Test::runTest(long max, std::shared_ptr<ISieve> sieve, std::ofstream& outFile) {
    if (!sieve) {
        std::cerr << "Sieve pointer is null!" << std::endl;
        return;
    }

    sieve->setMaxLimit(max);

    timer->start(); 
    sieve->computePrimes();
    timer->stop();

    outFile << "Time taken for n=" << max << ": " << timer->getTime() << "ms\n";

    timer.reset();  
}