#pragma once
#include "pch.h"
#include "mainScreen.h"

namespace export_ns {
    void exportScreen(std::vector<mainScreen::Transaction*> transactions,std::string account);
}