#include "accounts.h"

Account::Account(const std::string& user, const std::string& pass)
    : username(user), password(pass) {}

Account::~Account() {}

std::string encrypt(const std::string& text, char key) {
    std::string result = text;
    for (char& c : result) {
        c ^= key;
    }
    for (char& c : result) {
        c = static_cast<char>(c + (key % 26));
    }
    std::ranges::reverse(result.begin(), result.end());
    return result;
}

void Account::saveToFile(const std::string& filePath) const {
    std::ofstream file(filePath, std::ios_base::app);
    if (file.is_open()) {
        std::string encryptedPassword = encrypt(password, 'X');
        std::cout << username << "," << encryptedPassword << std::endl;
        file << username << "," << encryptedPassword << "\n";
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
