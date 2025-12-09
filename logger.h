#pragma once
#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include <string>
#include <memory>

class Logger {
private:
    static std::unique_ptr<Logger> instance;
    static std::mutex init_mtx;
    static std::mutex write_mtx;
    std::ofstream logFile;

    // 생성자: 파일 열고 Init 로그
    Logger(const std::string& filename = "Test/output2.txt") {
        logFile.open(filename, std::ios::trunc);
        if (logFile.is_open()) {
            logFile << "[Init] Logger started." << std::endl;
        }
    }

public:
    // 소멸자는 public 이어야 unique_ptr이 delete 가능
    ~Logger() {
        if (logFile.is_open()) {
            logFile << "[Shutdown] Logger closed." << std::endl;
            logFile.close();
        }
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger* getInstance(const std::string& filename = "Test/output2.txt") {
        if (!instance) {
            std::lock_guard<std::mutex> lock(init_mtx);
            if (!instance) {
                instance.reset(new Logger(filename));
            }
        }
        return instance.get();
    }

    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(write_mtx);   // 쓰기 동기화
        if (logFile.is_open()) {
            logFile << message << std::endl;
        }
    }
};

std::unique_ptr<Logger> Logger::instance;
std::mutex Logger::init_mtx;
std::mutex Logger::write_mtx;
