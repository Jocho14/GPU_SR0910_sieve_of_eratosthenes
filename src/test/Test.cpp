#include "test.hpp"
#include <typeinfo>
#include <string>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <memory>
#include <thread>
#include <vector>
#include <atomic>
#include <numeric>

float calculateCpuUsage(FILETIME prev_idle, FILETIME prev_kernel, FILETIME prev_user,
    FILETIME idle, FILETIME kernel, FILETIME user) {
    ULARGE_INTEGER prev_idle_time, prev_kernel_time, prev_user_time;
    ULARGE_INTEGER idle_time, kernel_time, user_time;

    prev_idle_time.LowPart = prev_idle.dwLowDateTime;
    prev_idle_time.HighPart = prev_idle.dwHighDateTime;

    prev_kernel_time.LowPart = prev_kernel.dwLowDateTime;
    prev_kernel_time.HighPart = prev_kernel.dwHighDateTime;

    prev_user_time.LowPart = prev_user.dwLowDateTime;
    prev_user_time.HighPart = prev_user.dwHighDateTime;

    idle_time.LowPart = idle.dwLowDateTime;
    idle_time.HighPart = idle.dwHighDateTime;

    kernel_time.LowPart = kernel.dwLowDateTime;
    kernel_time.HighPart = kernel.dwHighDateTime;

    user_time.LowPart = user.dwLowDateTime;
    user_time.HighPart = user.dwHighDateTime;

    ULONGLONG total_time_diff = (kernel_time.QuadPart - prev_kernel_time.QuadPart) +
        (user_time.QuadPart - prev_user_time.QuadPart);
    ULONGLONG idle_time_diff = idle_time.QuadPart - prev_idle_time.QuadPart;

    if (total_time_diff == 0) return 0.0f;

    return (float)(total_time_diff - idle_time_diff) / total_time_diff * 100.0f;
}

Test::Test() : timer_(std::make_shared<Timer>()) {}

void Test::runTimeTest(unsigned int max, std::shared_ptr<ISieve> sieve, std::ofstream& outFile) {
    if (!outFile.is_open()) {
        std::cerr << "Błąd otwierania pliku!" << std::endl;
        return;
    }

    std::vector<unsigned int> values = {
        max,
        max / 2,
        max / 10,
        max / 50,
        max / 100,
        max / 1000
    };

    for (const auto& value : values) {
        std::cout << "Przetwarzanie wartości: " << value << std::endl;
        try {
            sieve->setMaxLimit(value);
            for (int i = 0; i < 10; ++i) {
                std::cout << "Iteracja: " << i << " dla wartości " << value << std::endl;
                timer_->start();
                sieve->computePrimes();
                timer_->stop();

                double elapsed = timer_->getTime();
                std::cout << "Czas: " << elapsed << std::endl;

                outFile << value << ";" << elapsed << ";";
                timer_->reset();
            }
            outFile << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "Wyjątek: " << e.what() << " przy wartości " << value << std::endl;
        }
        catch (...) {
            std::cerr << "Nieznany wyjątek przy wartości " << value << std::endl;
        }
    }
}


void Test::runUsageTest(unsigned int max, std::shared_ptr<ISieve> sieve, std::ofstream& outFile) {
    if (!sieve) {
        std::cerr << "Sieve pointer is null!" << std::endl;
        return;
    }

    sieve->setMaxLimit(max);

    std::vector<float> cpu_usage_data;
    std::atomic<bool> stop_flag = false;

    // Wątek monitorujący CPU
    std::thread monitor_thread([&cpu_usage_data, &stop_flag]() {
        FILETIME prev_idle, prev_kernel, prev_user;
        FILETIME idle, kernel, user;

        GetSystemTimes(&prev_idle, &prev_kernel, &prev_user);

        bool initialized = false;

        while (!stop_flag) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            GetSystemTimes(&idle, &kernel, &user);

            if (initialized) {
                float cpu_usage = calculateCpuUsage(prev_idle, prev_kernel, prev_user, idle, kernel, user);
                cpu_usage_data.push_back(cpu_usage);
            }
            else {
                initialized = true;
            }

            prev_idle = idle;
            prev_kernel = kernel;
            prev_user = user;
        }
        });

    timer_->start();
    sieve->computePrimes();
    timer_->stop();

    stop_flag = true;
    monitor_thread.join();

    outFile << "Time taken for n=" << max << ": " << timer_->getTime() << "ms\n";
    outFile << "CPU Usage Data:\n";
    for (const auto& usage : cpu_usage_data) {
        outFile << usage << "%\n";
    }

    timer_.reset();
}
