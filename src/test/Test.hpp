#ifndef TEST_HPP
#define TEST_HPP

#include <iostream>
#include <memory>
#include <fstream>

#include "../common/timer/Timer.hpp"
#include "../interfaces/ISieve.hpp"

class Test {
public:
    Test();
    void runTimeTest(unsigned int max, std::shared_ptr<ISieve> sieve, std::ofstream& outFile);
    void runUsageTest(unsigned int max, std::shared_ptr<ISieve> sieve, std::ofstream& outFile);
private:
    std::shared_ptr<Timer> timer_;
};

#endif // TEST_HPP