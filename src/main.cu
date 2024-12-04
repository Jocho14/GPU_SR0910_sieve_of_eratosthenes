#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
#include <chrono>
#include <cuda_runtime.h>
#include "memory"
#include <string>

#include "common/timer/Timer.hpp"

#include "cpu/singleThreaded/SingleCpuSieve.hpp"
#include "cpu/multiThreaded/MultiCpuSieve.hpp"
#include "gpu/GpuSieve.hpp"
#include "../src/test/Test.hpp"

int main() {

	auto single = std::make_shared<SingleCpuSieve>(42949);
	auto multi = std::make_shared<MultiCpuSieve>(42949);
	auto gpu = std::make_shared<GpuSieve>();
	/*auto gpuNcite = std::make_shared<>();*/

	std::ofstream outFileSingleTime("singleTime.csv");
	std::ofstream outFileMultiTime("multiTime.csv");
	std::ofstream outFileGpuTime("GpuTime.csv");
	std::ofstream outFileGpuTimeNcite("GpuTimeNcite.csv");

	std::ofstream outFileSingleUsage("singleUsage.csv");
	std::ofstream outFileMultiUsage("multiUsage.csv");
	std::ofstream outFileGpuUsage("GpuUsage.csv");
	std::ofstream outFileGpuUsageNcite("GpuUsageNcite.csv");

	/*const unsigned int Max = 4294967285;*/
	const unsigned int Max = 42949;

	auto test = std::make_shared<Test>();

	std::cout << "start time test for singlecpusieve...\n";
	test->runTimeTest(Max, single, outFileSingleTime);
	std::cout << "stop time test for singlecpusieve.\n";
	
	std::cout << "start time test for multicpusieve...\n";
	test->runTimeTest(Max, multi, outFileMultiTime);
	std::cout << "stop time test for multicpusieve.\n";

	std::cout << "start time test for gpusieve...\n";
	test->runTimeTest(Max, gpu, outFileGpuTime);
	std::cout << "stop time test for gpusieve.\n";

	/*std::cout << "start time test for gpusieve after ncite...\n";
	test->runTimeTest(Max, gpuNcite, outFileGpuTimeNcite);
	std::cout << "stop time test for gpusieve after ncite.\n";*/

	std::cout << "start usage test for singlecpusieve...\n";
	test->runUsageTest(Max, single, outFileSingleUsage, false);
	std::cout << "stop usage test for singlecpusieve.\n";

	std::cout << "start usage test for multicpusieve...\n";
	test->runUsageTest(Max, multi, outFileMultiUsage, false);
	std::cout << "stop usage test for multicpusieve.\n";

	//std::cout << "Start usage test for GpuSieve...\n";
	//test->runUsageTest(Max, gpu, outFileGpuUsage, true);
	//std::cout << "Stop usage test for GpuSieve.\n";

	/*std::cout << "start time test for gpusieve after ncite...\n";
	test->runTimeTest(Max, gpuNcite, outFileGpuUsageNcite, true);
	std::cout << "stop time test for gpusieve after ncite.\n";*/

	return 0;
}