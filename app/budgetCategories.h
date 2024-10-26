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

    void printCategory(Category category, int Yoffset);
    void setDefaults(Category *def[]);
    void categoryScreen(std::string account);

}