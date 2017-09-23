#include <iostream>
#include "spdlog/spdlog.h"

int main(int, char* [])
{
    //Multithreaded console logger(with color support)
    try 
    {
        // Create basic file logger (not rotated)
        auto my_logger1 = spdlog::basic_logger_mt("basic_logger1", "logs/basic1.txt");
        auto my_logger2 = spdlog::basic_logger_mt("basic_logger2", "logs/basic2.txt");
        my_logger1->info("Hello");
        my_logger2->info("world");
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
    return 0;
}
