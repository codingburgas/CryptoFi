#pragma once
#include "pch.h"

namespace mainScreen {
    struct Transaction {
        std::string type;
        std::string reason;
        std::chrono::system_clock::time_point date;
    };

    void mainScreen(std::string account);
    void updateMoney(float* money, const std::string& account, std::vector<Transaction*>& transactions);
    std::chrono::system_clock::time_point* parseDate(const std::string* dateStr);
    std::string formatDate(const std::chrono::system_clock::time_point& date);
}
