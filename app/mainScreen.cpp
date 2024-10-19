#include "mainScreen.h"
#include "accounts.h"

namespace mainScreen {
    void updateMoney(float* money, const std::string& account) {
    std::string* newAmountInput = new std::string;
    std::string* reasonInput = new std::string;
    bool typingAmount = true;
    bool typingReason = false;

    while (true) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Update Money", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 150, 30, LIGHTGRAY);
        DrawText("Enter new amount:", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 100, 20, GRAY);
        DrawText(newAmountInput->c_str(), GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 60, 20, DARKGRAY);

        DrawText("Enter reason:", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2, 20, GRAY);
        DrawText(reasonInput->c_str(), GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 40, 20, DARKGRAY);

        if (IsKeyPressed(KEY_TAB)) {
            typingAmount = !typingAmount;
            typingReason = !typingReason;
        }

        int key = GetCharPressed();
        if (key > 0) {
            if (typingAmount) {
                *newAmountInput += static_cast<char>(key);
            } else if (typingReason) {
                *reasonInput += static_cast<char>(key);
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (typingAmount && !newAmountInput->empty()) {
                newAmountInput->pop_back();
            } else if (typingReason && !reasonInput->empty()) {
                reasonInput->pop_back();
            }
        }

        if (IsKeyPressed(KEY_ENTER) && !newAmountInput->empty() && !reasonInput->empty()) {
            float newAmount = std::stof(*newAmountInput);

            std::string type = (newAmount > *money) ? "revenue" : "expense";

            auto now = std::chrono::system_clock::now();
            std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
            std::string date = std::ctime(&nowTime);
            date.pop_back();

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
                lines[0] = "money,"+std::to_string(newAmount);
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
                fileAppend << type << "," << *reasonInput << "," << date << "\n";
                fileAppend.close();
            } else {
                std::cerr << "Failed to open file: " << csvFilePath << std::endl;
            }

            *money = newAmount;

            break;
        }

        EndDrawing();
    }

    delete newAmountInput;
    delete reasonInput;
}

    void mainScreen(std::string account) {
        float* money = new float(0.0f);
        float* budget = new float(0.0f);

        std::string csvFilePath = "data/profiles/" + account + "_profile.csv";

        std::ifstream file(csvFilePath);
        if (file.is_open()) {
            std::string line;
            int lineNumber = 0;

            while (std::getline(file, line)) {
                std::istringstream ss(line);
                std::string key;
                float value;

                if (std::getline(ss, key, ',') && ss >> value) {
                    if (lineNumber == 0) {
                        *money = value;  // Load money
                    } else if (lineNumber == 1) {
                        *budget = value;  // Load budget
                    }
                }
                lineNumber++;
            }
            file.close();
        } else {
            std::cerr << "Failed to open file: " << csvFilePath << std::endl;
        }

        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawText(account.c_str(), 50, 20, 20, GRAY);
            DrawText(TextFormat("Money: %.2f", *money), 200, 20, 20, GRAY);
            DrawText(TextFormat("Budget: %.2f", *budget), 350, 20, 20, GRAY);
            if(CheckCollisionPointRec(GetMousePosition(),{200,20,125,20}) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
                updateMoney(money, account);
            }
            EndDrawing();
        }

        delete money;
        delete budget;
    }
}