#include "mainScreen.h"
#include "accounts.h"

namespace mainScreen {

    void sortTransactions(std::vector<Transaction*>& transactions) {
        size_t n = transactions.size();
        for (size_t i = 0; i < n - 1; ++i) {
            for (size_t j = 0; j < n - i - 1; ++j) {
                if (transactions[j]->date < transactions[j + 1]->date) {
                    std::swap(transactions[j], transactions[j + 1]);
                }
            }
        }
    }

    void updateMoney(float* money, const std::string& account, std::vector<Transaction*>& transactions) {
    std::string* differenceInput = new std::string;
    std::string* reasonInput = new std::string;
    bool* typingDifference = new bool(true);
    bool* typingReason = new bool(false);
    bool* isRevenue = new bool(true);

    while (true) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            break;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Update Money", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 150, 30, LIGHTGRAY);

        DrawText("Transaction Type:", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 120, 20, GRAY);
        DrawText(*isRevenue ? "Revenue (+)" : "Expense (-)", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 90, 20, DARKGRAY);

        DrawText("Enter amount difference:", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 60, 20, GRAY);
        DrawText(differenceInput->c_str(), GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 30, 20, DARKGRAY);

        DrawText("Enter reason:", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 10, 20, GRAY);
        DrawText(reasonInput->c_str(), GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 40, 20, DARKGRAY);

        if (IsKeyPressed(KEY_TAB)) {
            *typingDifference = !(*typingDifference);
            *typingReason = !(*typingReason);
        }

        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_LEFT) ||
    (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) &&
    CheckCollisionPointRec(GetMousePosition(),
    {static_cast<float>(GetScreenWidth() / 2 - 100), static_cast<float>(GetScreenHeight() / 2 - 30), 200, 30})) {
            *typingDifference = 1;
            *typingReason = 0;
        }

        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_LEFT) ||
    (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) &&
    CheckCollisionPointRec(GetMousePosition(),
    {static_cast<float>(GetScreenWidth() / 2 - 100), static_cast<float>(GetScreenHeight() / 2 +40), 200, 30})) {
            *typingDifference = 0;
            *typingReason = 1;
        }


        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_LEFT) ||
    (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) &&
    CheckCollisionPointRec(GetMousePosition(),
    {static_cast<float>(GetScreenWidth() / 2 - 100), static_cast<float>(GetScreenHeight() / 2 - 90), 200, 20})) {
            *isRevenue = !(*isRevenue);
    }


        int key = GetCharPressed();
        if (key > 0) {
            if (*typingDifference) {
                *differenceInput += static_cast<char>(key);
            } else if (*typingReason) {
                *reasonInput += static_cast<char>(key);
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (*typingDifference && !differenceInput->empty()) {
                differenceInput->pop_back();
            } else if (*typingReason && !reasonInput->empty()) {
                reasonInput->pop_back();
            }
        }

        if (IsKeyPressed(KEY_ENTER) && !differenceInput->empty() && !reasonInput->empty()) {
            float difference = std::stof(*differenceInput);
            if (!*isRevenue) {
                difference = -difference;
            }

            *money += difference;

            std::string* type = new std::string(*isRevenue ? "revenue" : "expense");

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
            sortTransactions(transactions);
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
                lines[0] = "money," + std::to_string(*money);
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

            // Clean up
            delete differenceInput;
            delete reasonInput;
            delete typingDifference;
            delete typingReason;
            delete type;
            delete date;
            delete isRevenue;

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

    Camera2D camera = {0};
    camera.target = {0, 0};
    camera.offset = {0, 0};
    camera.zoom = 1.0f;

    float scrollSpeed = 30.0f;
    float screenHeight = static_cast<float>(GetScreenHeight());
    float transactionHeight = 30.0f;
    float baseY = 100.0f;
    float transactionAreaHeight = transactions.size() * transactionHeight;

    float maxScroll = transactionAreaHeight - (screenHeight - baseY);

    if (maxScroll < 0) {
        maxScroll = 0;
    }
    sortTransactions(transactions);

        bool* showNavigationsBar = new bool(false);

    while (!WindowShouldClose()) {
        camera.target.y -= GetMouseWheelMove() * scrollSpeed;

        if (camera.target.y < 0) {
            camera.target.y = 0;
        }
        if (camera.target.y > maxScroll) {
            camera.target.y = maxScroll;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText(account.c_str(), 50, 20, 20, GRAY);
        DrawText(TextFormat("Money: %.2f", *money), 200, 20, 20, GRAY);
        DrawText(TextFormat("Budget: %.2f", *budget), 350, 20, 20, GRAY);

        BeginMode2D(camera);

        for (int i = 0; i < transactions.size(); i++) {
            Transaction* t = transactions[i];
            std::string displayText = t->type + ": " + t->reason + " - " + formatDate(t->date) + " (Difference: " + formatDifference(t->difference) + ")";
            DrawText(displayText.c_str(), 50, baseY + i * transactionHeight, 20, DARKGRAY);
        }

        EndMode2D();

        DrawLine(GetScreenWidth()-80,GetScreenHeight()/16,GetScreenWidth()-40,GetScreenHeight()/16,GRAY);
        DrawLine(GetScreenWidth()-80,GetScreenHeight()/16-10,GetScreenWidth()-40,GetScreenHeight()/16-10,GRAY);
        DrawLine(GetScreenWidth()-80,GetScreenHeight()/16-20,GetScreenWidth()-40,GetScreenHeight()/16-20,GRAY);

        if(CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth()-80),static_cast<float>(GetScreenHeight()/16-20),40,20}) &&
            (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
            *showNavigationsBar = !(*showNavigationsBar);
            }

        if(*showNavigationsBar == true){
            DrawRectangle(GetScreenWidth()/1.15,GetScreenHeight()/7.8,150,300,GRAY);
            DrawText("Budget",GetScreenWidth()/1.15+5,GetScreenHeight()/7.8+20,20,WHITE);
            DrawText("Analytics",GetScreenWidth()/1.15+5,GetScreenHeight()/7.8+40,20,WHITE);

            if(CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth()/1.15+5),static_cast<float>(GetScreenHeight()/7.8+40),40,20}) &&
            (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
                std::cout << "Analytics" << std::endl;
            }

        }

        if (CheckCollisionPointRec(GetMousePosition(), {200, 20, 125, 20}) &&
            (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
            updateMoney(money, account, transactions);
        }

        EndDrawing();
    }

    delete money;
    delete budget;
    delete showNavigationsBar;
    for (int i = 0; i < transactions.size(); i++) {
        delete transactions[i];
    }
}

}