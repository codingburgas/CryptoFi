#pragma once
#include "pch.h"
#include "mainScreen.h"

namespace export_ns {
    void exportScreen(std::vector<mainScreen::Transaction*> transactions,  std::string& account, float* money);
    std::chrono::system_clock::time_point* parseDateWithTime(const std::string* dateStr, bool isFromDate);
}