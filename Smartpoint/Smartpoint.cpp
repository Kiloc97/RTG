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
    // ���� �̸��� ofstream ����Ʈ ������ ����
    std::map<std::string, std::unique_ptr<std::ofstream>> fileMap;

    // Ÿ�ӽ����� ���� �Լ�
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
        // ��� ������ �����ϰ� �ݱ�
        for (auto& pair : fileMap) {
            if (pair.second && pair.second->is_open()) {
                pair.second->close();
            }
        }
    }

    // ����/�̵� ����
    LogFileManager(const LogFileManager&) = delete;
    LogFileManager& operator=(const LogFileManager&) = delete;

    // �α� ���� ����
    void openLogFile(const std::string& filename) {
        if (fileMap.count(filename) == 0) {
            auto ofs = std::make_unique<std::ofstream>(filename, std::ios::app);
            if (!ofs->is_open()) {
                throw std::runtime_error("���� ���� ����: " + filename);
            }
            fileMap[filename] = std::move(ofs);
        }
    }

    // �α� ���Ͽ� �޽��� ���� (Ÿ�ӽ����� ����)
    void writeLog(const std::string& filename, const std::string& message) {
        auto it = fileMap.find(filename);
        if (it == fileMap.end() || !it->second || !it->second->is_open()) {
            throw std::runtime_error("������ �������� �ʽ��ϴ�: " + filename);
        }
        *(it->second) << currentTimestamp() << " " << message << std::endl;
    }

    // �α� ���Ͽ��� ��ü ���ε��� vector�� ��ȯ
    std::vector<std::string> readLogs(const std::string& filename) {
        std::vector<std::string> result;
        std::ifstream ifs(filename);
        if (!ifs.is_open()) {
            throw std::runtime_error("���� �б� ����: " + filename);
        }
        std::string line;
        while (std::getline(ifs, line)) {
            if (!line.empty()) result.push_back(line);
        }
        return result;
    }

    // �α� ���� �ݱ�
    void closeLogFile(const std::string& filename) {
        auto it = fileMap.find(filename);
        if (it != fileMap.end() && it->second && it->second->is_open()) {
            it->second->close();
            fileMap.erase(it);
        }
    }
};

int main() {
    try {
        LogFileManager manager;
        manager.openLogFile("error.log");
        manager.openLogFile("debug.log");
        manager.openLogFile("info.log");

        manager.writeLog("error.log", "Database connection failed");
        manager.writeLog("error.log", "Database connection failed2");
        manager.writeLog("error.log", "Database connection failed3");
        manager.writeLog("debug.log", "User login attempt");
        manager.writeLog("info.log", "Server started successfully");

        std::vector<std::string> errorLogs = manager.readLogs("error.log");
        std::vector<std::string> debugLogs = manager.readLogs("debug.log");
        std::vector<std::string> infoLogs = manager.readLogs("info.log");

        // �α� ��� �߰�
        for (const auto& line : errorLogs) {
            std::cout << line << std::endl; 
        }
        for (const auto& line : debugLogs) {
            std::cout << line << std::endl; 
        }
        for (const auto& line : infoLogs) {
            std::cout << line << std::endl;
        }

        manager.closeLogFile("error.log");
        manager.closeLogFile("debug.log");
        manager.closeLogFile("info.log");
    } catch (const std::exception& ex) {
        std::cerr << "���� �߻�: " << ex.what() << std::endl;
        return 1;
    }
    
    while(1){
        // ���� ����
        //q: ����
        char c = std::cin.get();
        if(c == 'q'){
            break;
        }
    }
    return 0;
}

