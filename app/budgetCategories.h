#pragma once
#include "pch.h"

namespace budgetCategories {
    class Category {
    public:
        std::string type;
        std::string name;
        std::string description;
        float budget;
        int dueYear;
        int dueMonth;
        int dueDay;
        int dueHour;
        int dueMinute;
    };
    void printCategory(Category category, int Yoffset);
    void setDefaults(Category *def[]);
    void categoryScreen(std::string account);

}