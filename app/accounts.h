#pragma once
#include "pch.h"
class Account {
public:
    Account(const std::string& user, const std::string& pass);
    ~Account();

    void saveToFile(const std::string& filePath) const;
    std::string getUsername() const;
    std::string getPassword() const;
private:
    std::string username;
    std::string password;
};
std::string encrypt(const std::string& text, char key);
