#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <stdexcept>

class LogFileManager {
private:
    // 파일 이름별 ofstream 스마트 포인터 관리
    std::map<std::string, std::unique_ptr<std::ofstream>> fileMap;

    // 타임스탬프 생성 함수
    std::string currentTimestamp() {
        std::ostringstream oss;
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        oss << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "]";
        return oss.str();
    }

public:
    LogFileManager() = default;
    ~LogFileManager() {
        // 모든 파일을 안전하게 닫기
        for (auto& pair : fileMap) {
            if (pair.second && pair.second->is_open()) {
                pair.second->close();
            }
        }
    }

    // 복사/이동 금지
    LogFileManager(const LogFileManager&) = delete;
    LogFileManager& operator=(const LogFileManager&) = delete;

    // 로그 파일 열기
    void openLogFile(const std::string& filename) {
        if (fileMap.count(filename) == 0) {
            auto ofs = std::make_unique<std::ofstream>(filename, std::ios::app);
            if (!ofs->is_open()) {
                throw std::runtime_error("파일 열기 실패: " + filename);
            }
            fileMap[filename] = std::move(ofs);
        }
    }

    // 로그 파일에 메시지 쓰기 (타임스탬프 포함)
    void writeLog(const std::string& filename, const std::string& message) {
        auto it = fileMap.find(filename);
        if (it == fileMap.end() || !it->second || !it->second->is_open()) {
            throw std::runtime_error("파일이 열려있지 않습니다: " + filename);
        }
        *(it->second) << currentTimestamp() << " " << message << std::endl;
    }

    // 로그 파일에서 전체 라인들을 vector로 반환
    std::vector<std::string> readLogs(const std::string& filename) {
        std::vector<std::string> result;
        std::ifstream ifs(filename);
        if (!ifs.is_open()) {
            throw std::runtime_error("파일 읽기 실패: " + filename);
        }
        std::string line;
        while (std::getline(ifs, line)) {
            if (!line.empty()) result.push_back(line);
        }
        return result;
    }

    // 로그 파일 닫기
    void closeLogFile(const std::string& filename) {
        auto it = fileMap.find(filename);
        if (it != fileMap.end() && it->second && it->second->is_open()) {
            it->second->close();
            fileMap.erase(it);
        }
    }
};

int main() {
    LogFileManager manager;
    manager.openLogFile("error.log");
    manager.openLogFile("debug.log");
    manager.openLogFile("info.log");

    manager.writeLog("error.log", "Database connection failed");
    manager.writeLog("debug.log", "User login attempt");
    manager.writeLog("info.log", "Server started successfully");

    std::vector<std::string> errorLogs = manager.readLogs("error.log");
    
    for (const auto& line : errorLogs) {
        
    }

    manager.closeLogFile("error.log");
    manager.closeLogFile("debug.log");
    manager.closeLogFile("info.log");
    return 0;
}

