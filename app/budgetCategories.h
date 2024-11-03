#pragma once
#include "pch.h"

namespace budgetCategories {
    struct Category {
        std::string type;
        std::string name;
        std::string description;
        float budget;
        int lineNumber;
    };

    void categoryScreen(std::string account);
    void newBudget(std::string account, std::vector<Category>& categories);
    void updateCategory(std::string account, Category& category, std::vector<Category>& categories);
}