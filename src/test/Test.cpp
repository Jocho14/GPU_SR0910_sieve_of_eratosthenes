#include "test.hpp"
#include <typeinfo>

Test::Test() : timer_(std::make_shared<Timer>()) {}


void Test::runTest(long max, std::shared_ptr<ISieve> sieve, std::ofstream& outFile) {
    if (!sieve) {
        std::cerr << "Sieve pointer is null!" << std::endl;
        return;
    }
    std::cout << "Object type: " << typeid(*sieve).name() << std::endl;

    sieve->setMaxLimit(max);

    timer_->start(); 
    sieve->computePrimes();
    timer_->stop();

    outFile << "Time taken for n=" << max << ": " << timer_->getTime() << "ms\n";


    timer_.reset();  
}