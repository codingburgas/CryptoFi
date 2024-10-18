#include "accounts.h"

Account::Account(const std::string& user, const std::string& pass)
    : username(user), password(pass) {}

Account::~Account() {}

void Account::saveToFile(const std::string& filePath) const {
    std::ofstream file(filePath, std::ios_base::app);
    if (file.is_open()) {
        std::cout << username << "," << password << std::endl;
        file << username << "," << password << "\n";
        file.close();
    } else {
        std::cerr << "Unable to open file\n";
    }
}


std::string Account::getUsername() const {
    return username;
}

std::string Account::getPassword() const {
    return password;
}
