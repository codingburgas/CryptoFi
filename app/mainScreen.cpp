#include "mainScreen.h"
#include "accounts.h"

namespace mainScreen {
    void updateMoney(float* money, const std::string& account, std::vector<Transaction*>& transactions) {
    std::string* newAmountInput = new std::string;
    std::string* reasonInput = new std::string;
    bool* typingAmount = new bool(true);
    bool* typingReason = new bool(false);

    while (true) {
        if(IsKeyPressed(KEY_ESCAPE)) {
            break;
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Update Money", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 150, 30, LIGHTGRAY);
        DrawText("Enter new amount:", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 100, 20, GRAY);
        DrawText(newAmountInput->c_str(), GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 60, 20, DARKGRAY);

        DrawText("Enter reason:", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2, 20, GRAY);
        DrawText(reasonInput->c_str(), GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 40, 20, DARKGRAY);

        if (IsKeyPressed(KEY_TAB)) {
            *typingAmount = !(*typingAmount);
            *typingReason = !(*typingReason);
        }

        int key = GetCharPressed();
        if (key > 0) {
            if (*typingAmount) {
                *newAmountInput += static_cast<char>(key);
            } else if (*typingReason) {
                *reasonInput += static_cast<char>(key);
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (*typingAmount && !newAmountInput->empty()) {
                newAmountInput->pop_back();
            } else if (*typingReason && !reasonInput->empty()) {
                reasonInput->pop_back();
            }
        }

        if (IsKeyPressed(KEY_ENTER) && !newAmountInput->empty() && !reasonInput->empty()) {
            float newAmount = std::stof(*newAmountInput);
            float difference = newAmount - *money;

            std::string* type = new std::string((newAmount > *money) ? "revenue" : "expense");

            auto now = std::chrono::system_clock::now();
            std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
            std::string* date = new std::string(std::ctime(&nowTime));
            date->pop_back();

            Transaction* newTransaction = new Transaction;
            newTransaction->type = *type;
            newTransaction->reason = *reasonInput;
            newTransaction->date = now;
            newTransaction->difference = difference;
            transactions.push_back(newTransaction);

            std::string csvFilePath = "data/profiles/" + account + "_profile.csv";
            std::vector<std::string> lines;
            std::ifstream fileIn(csvFilePath);

            if (fileIn.is_open()) {
                std::string line;
                while (std::getline(fileIn, line)) {
                    lines.push_back(line);
                }
                fileIn.close();
            } else {
                std::cerr << "Failed to open file: " << csvFilePath << std::endl;
            }

            if (!lines.empty()) {
                lines[0] = "money," + std::to_string(newAmount);
            }

            std::ofstream fileOut(csvFilePath, std::ios::trunc);
            if (fileOut.is_open()) {
                for (const auto& line : lines) {
                    fileOut << line << "\n";
                }
                fileOut.close();
            } else {
                std::cerr << "Failed to write to file: " << csvFilePath << std::endl;
            }

            std::ofstream fileAppend(csvFilePath, std::ios::app);
            if (fileAppend.is_open()) {
                fileAppend << *type << "," << *reasonInput << "," << *date << "," << difference << "\n";
                fileAppend.close();
            } else {
                std::cerr << "Failed to open file: " << csvFilePath << std::endl;
            }

            *money = newAmount;

            delete newAmountInput;
            delete reasonInput;
            delete typingAmount;
            delete typingReason;
            delete type;
            delete date;

            break;
        }

        EndDrawing();
    }
}


    std::chrono::system_clock::time_point* parseDate(const std::string* dateStr) {
        std::tm tm = {};
        std::istringstream ss(*dateStr);
        ss >> std::get_time(&tm, "%a %b %d %H:%M:%S %Y");
        if (ss.fail()) {
            std::cerr << "Failed to parse date: " << *dateStr << std::endl;
            return new std::chrono::system_clock::time_point(std::chrono::system_clock::now());
        }
        std::time_t time = std::mktime(&tm);
        return new std::chrono::system_clock::time_point(std::chrono::system_clock::from_time_t(time));
    }

    std::string formatDate(const std::chrono::system_clock::time_point& date) {
        std::time_t time = std::chrono::system_clock::to_time_t(date);
        std::tm* tm = std::localtime(&time);
        char buffer[30];
        std::strftime(buffer, sizeof(buffer), "%a %b %d %H:%M:%S %Y", tm);
        return std::string(buffer);
    }

    std::string formatDifference(float difference) {
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(2) << difference;
        return stream.str();
    }

    void mainScreen(std::string account) {
    float* money = new float(0.0f);
    float* budget = new float(0.0f);
    std::vector<Transaction*> transactions;

    std::string* csvFilePath = new std::string("data/profiles/" + account + "_profile.csv");

    std::ifstream* file = new std::ifstream(*csvFilePath);
    if (file->is_open()) {
        std::string* line = new std::string;
        int* lineNumber = new int(0);

        while (std::getline(*file, *line)) {
            std::istringstream* ss = new std::istringstream(*line);
            std::string* key = new std::string;
            float* value = new float(0.0f);

            if (std::getline(*ss, *key, ',') && (*ss >> *value)) {
                if (*lineNumber == 0) {
                    *money = *value;
                } else if (*lineNumber == 1) {
                    *budget = *value;
                }
            }

            if (line->find("revenue") == 0 || line->find("expense") == 0) {
                std::string* type = new std::string;
                std::string* reason = new std::string;
                std::string* dateStr = new std::string;
                float* difference = new float(0.0f);

                std::istringstream* transactionStream = new std::istringstream(*line);
                std::getline(*transactionStream, *type, ',');
                std::getline(*transactionStream, *reason, ',');
                std::getline(*transactionStream, *dateStr, ',');
                *transactionStream >> *difference;

                Transaction* t = new Transaction;
                t->type = *type;
                t->reason = *reason;
                t->date = *parseDate(dateStr);
                t->difference = *difference;
                transactions.push_back(t);

                delete type;
                delete reason;
                delete dateStr;
                delete difference;
                delete transactionStream;
            }

            (*lineNumber)++;
            delete ss;
        }

        file->close();
        delete line;
        delete lineNumber;
    } else {
        std::cerr << "Failed to open file: " << *csvFilePath << std::endl;
    }

    delete csvFilePath;
    delete file;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText(account.c_str(), 50, 20, 20, GRAY);
        DrawText(TextFormat("Money: %.2f", *money), 200, 20, 20, GRAY);
        DrawText(TextFormat("Budget: %.2f", *budget), 350, 20, 20, GRAY);

        for (int i = 0; i < transactions.size(); i++) {
            Transaction* t = transactions[i];
            std::string displayText = t->type + ": " + t->reason + " - " + formatDate(t->date) + " (Difference: " + formatDifference(t->difference) + ")";
            DrawText(displayText.c_str(), 50, 100 + i * 30, 20, DARKGRAY);
        }


        if (CheckCollisionPointRec(GetMousePosition(), {200, 20, 125, 20}) &&
            (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
            updateMoney(money, account, transactions);
        }

        EndDrawing();
    }

    delete money;
    delete budget;

    for (int i = 0; i < transactions.size(); i++) {
        delete transactions[i];
    }
}

}