#pragma once
#include "pch.h"

namespace mainScreen {
    struct Transaction {
        std::string type;
        std::string reason;
        std::chrono::system_clock::time_point date;
        float difference;
    };

    void mainScreen(std::string account, Font *fontAll);
    void updateMoney(float* money, const std::string& account, std::vector<Transaction*>& transactions, Font *fontAll);
    std::chrono::system_clock::time_point* parseDate(const std::string* dateStr);
    std::string formatDate(const std::chrono::system_clock::time_point& date);
}
