#include "iostream"
#include "memory"
#include <typeinfo>
#include <string>

#include "common/timer/Timer.hpp"

#include "cpu/singleThreaded/SingleCpuSieve.hpp"
#include "cpu/multiThreaded/MultiCpuSieve.hpp"
#include "../src/test/Test.hpp"


int main()
{

	auto single = std::make_shared<SingleCpuSieve>(42949);
	auto multi = std::make_shared<MultiCpuSieve>(42949);
	auto gpu = std::make_shared<GpuSieve>(42949);
	std::ofstream outFileSingleTime("singleTime.csv");
	std::ofstream outFileMultiTime("multiTime.csv");

	/*const unsigned int Max = 4294967285;*/
	const unsigned int Max = 42949;

	auto test = std::make_shared<Test>();

	test->runTimeTest(Max, single, outFileSingleTime);

	return 0;
}
