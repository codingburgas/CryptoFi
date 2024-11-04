#pragma once
#include "pch.h"
#include "mainScreen.h"

namespace analytics {
    void analytics(std::vector<mainScreen::Transaction*> transactions, std::string& account, float* money, Font *fontAll);
}

