#pragma once
#include "pch.h"

namespace budgetCategories {
    class Category {
    public:
        std::string type;
        std::string name;
        std::string description;
        float budget;
        std::chrono::system_clock::time_point date;
    };

    void displaySelected(Category *category, bool* isSelected);
    void setDefaults(Category *def[]);
    void categoryScreen(std::string account);
}