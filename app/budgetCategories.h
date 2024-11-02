#pragma once
#include "pch.h"

namespace budgetCategories {
    struct Category {
        std::string type;
        std::string name;
        std::string description;
        float budget;
    };

    void categoryScreen(std::string account);
    void newBudget(std::string account, std::vector<Category>& categories);
}