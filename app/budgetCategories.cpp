#include "budgetCategories.h"
#include "mainScreen.h"

namespace budgetCategories {

        void newBudget(std::string account, std::vector<Category>& categories, Font *fontAll) {
        std::string typeInput;
        std::string nameInput;
        std::string descriptionInput;
        std::string budgetInput;
        bool typingType = true;
        bool typingName = false;
        bool typingDescription = false;
        bool typingBudget = false;

        while (true) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                break;
            }

            BeginDrawing();
            ClearBackground({51, 58, 63, 100});

            DrawRectangleRounded({static_cast<float>(GetScreenWidth() / 5), static_cast<float>(GetScreenHeight() / 6), static_cast<float>(GetScreenWidth()/1.8), static_cast<float>(GetScreenHeight()/2)}, 0.3f, 20, WHITE);

            DrawTextEx(*fontAll, "Enter Category Type:", {static_cast<float>(GetScreenWidth() / 4), static_cast<float>(GetScreenHeight() / 4) }, 30, 1, BLACK);
            DrawTextEx(*fontAll, typeInput.c_str(), { static_cast<float>( GetScreenWidth() / 2.28 ), static_cast<float> ( GetScreenHeight() / 4 )}, 30, 1, DARKGRAY);

            DrawTextEx(*fontAll, "Enter Category Name:", { static_cast<float>(GetScreenWidth() / 4), static_cast<float>(GetScreenHeight() / 3.2)}, 30, 1, BLACK);
            DrawTextEx(*fontAll, nameInput.c_str(), { static_cast<float>(GetScreenWidth() / 2.25), static_cast<float>(GetScreenHeight() / 3.2) }, 30, 1, DARKGRAY);

            DrawTextEx(*fontAll, "Enter Category Description:", { static_cast<float>(GetScreenWidth() / 4), static_cast<float>(GetScreenHeight() / 2.6)}, 30, 1,BLACK);
            DrawTextEx(*fontAll, descriptionInput.c_str(), { static_cast<float>(GetScreenWidth() / 2), static_cast<float>(GetScreenHeight() / 2.6)}, 30, 1, DARKGRAY);

            DrawTextEx(*fontAll, "Enter Category Budget:", { static_cast<float>(GetScreenWidth() / 4), static_cast<float>(GetScreenHeight() / 2.2) }, 30, 1, BLACK);
            DrawTextEx(*fontAll, budgetInput.c_str(), { static_cast<float>(GetScreenWidth() / 2.18), static_cast<float>(GetScreenHeight() / 2.2) }, 30, 1, DARKGRAY);

            if (IsKeyPressed(KEY_TAB)) {
                if (typingType) {
                    typingType = false;
                    typingName = true;
                } else if (typingName) {
                    typingName = false;
                    typingDescription = true;
                } else if (typingDescription) {
                    typingDescription = false;
                    typingBudget = true;
                } else if (typingBudget) {
                    typingBudget = false;
                    typingType = true;
                }
            }

            int key = GetCharPressed();
            if (key > 0) {
                if (typingType) {
                    typeInput += static_cast<char>(key);
                } else if (typingName) {
                    nameInput += static_cast<char>(key);
                } else if (typingDescription) {
                    descriptionInput += static_cast<char>(key);
                } else if (typingBudget) {
                    budgetInput += static_cast<char>(key);
                }
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (typingType && !typeInput.empty()) {
                    typeInput.pop_back();
                } else if (typingName && !nameInput.empty()) {
                    nameInput.pop_back();
                } else if (typingDescription && !descriptionInput.empty()) {
                    descriptionInput.pop_back();
                } else if (typingBudget && !budgetInput.empty()) {
                    budgetInput.pop_back();
                }
            }

            DrawRectangleRounded({static_cast<float>(GetScreenWidth()/2.8), static_cast<float>(GetScreenHeight()/1.8), 250, 40}, 0.3f, 20, BLACK);
            DrawTextEx(*fontAll, "Enter to save", { static_cast<float>(GetScreenWidth() / 2.8 + 50), static_cast<float>(GetScreenHeight() / 1.8)}, 30  , 1,  WHITE);
            if (IsKeyPressed(KEY_ENTER) || (CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth()/2.8), static_cast<float>(GetScreenHeight()/1.8), 250, 40}) &&
                         (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) && !typeInput.empty() && !nameInput.empty() && !descriptionInput.empty() && !budgetInput.empty()) {
                Category newCategory;
                newCategory.type = typeInput;
                newCategory.name = nameInput;
                newCategory.description = descriptionInput;
                newCategory.budget = std::stof(budgetInput);

                categories.push_back(newCategory);

                std::string csvFilePath = "data/profiles/" + account + "_profile.csv";
                std::ofstream file(csvFilePath, std::ios::app);
                if (file.is_open()) {
                    file << "category," << newCategory.type << "," << newCategory.name << "," << newCategory.description << "," << newCategory.budget << "\n";
                    file.close();
                } else {
                    std::cerr << "Failed to open file: " << csvFilePath << std::endl;
                }

                break;
            }

            EndDrawing();
        }
    }

    void updateCategory(std::string account, Category& category, std::vector<Category>& categories, Font *fontAll) {
        std::string typeInput = category.type;
        std::string nameInput = category.name;
        std::string descriptionInput = category.description;
        std::string budgetInput = std::to_string(category.budget);
        bool typingType = true;
        bool typingName = false;
        bool typingDescription = false;
        bool typingBudget = false;

        while (true) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                break;
            }

            BeginDrawing();
            ClearBackground({51, 58, 63, 100});

            DrawRectangleRounded({static_cast<float>(GetScreenWidth()/5), static_cast<float>(GetScreenHeight()/6), static_cast<float>(GetScreenWidth()/1.8), static_cast<float>(GetScreenHeight()/2)}, 0.3f, 20, WHITE);

            DrawTextEx(*fontAll, "Enter Category Type:", {static_cast<float>(GetScreenWidth() / 4), static_cast<float>(GetScreenHeight() / 4) }, 30, 1, BLACK);
            DrawTextEx(*fontAll, typeInput.c_str(), { static_cast<float>(GetScreenWidth() / 2.28), static_cast<float>(GetScreenHeight() / 4)}, 30, 1, DARKGRAY);

            DrawTextEx(*fontAll, "Enter Category Name:", { static_cast<float>(GetScreenWidth() / 4), static_cast<float>(GetScreenHeight() / 3.2)}, 30, 1, BLACK);
            DrawTextEx(*fontAll, nameInput.c_str(), { static_cast<float>( GetScreenWidth() / 2.25), static_cast<float>(GetScreenHeight() / 3.2)}, 30, 1, DARKGRAY);

            DrawTextEx(*fontAll, "Enter Category Description:", { static_cast<float>( GetScreenWidth() / 4), static_cast<float>( GetScreenHeight() / 2.6 ) }, 30, 1, BLACK);
            DrawTextEx(*fontAll, descriptionInput.c_str(), { static_cast<float>(GetScreenWidth() / 2), static_cast<float>(GetScreenHeight() / 2.6)}, 30, 1, DARKGRAY);

            DrawTextEx(*fontAll, "Enter Category Budget:", {static_cast<float>(GetScreenWidth() / 4), static_cast<float>(GetScreenHeight() / 2.2)}, 30, 1, BLACK);
            DrawTextEx(*fontAll, budgetInput.c_str(), { static_cast<float>( GetScreenWidth() / 2.18 ), static_cast<float>(GetScreenHeight() / 2.2)}, 30, 1, DARKGRAY);

            if (IsKeyPressed(KEY_TAB)) {
                if (typingType) {
                    typingType = false;
                    typingName = true;
                } else if (typingName) {
                    typingName = false;
                    typingDescription = true;
                } else if (typingDescription) {
                    typingDescription = false;
                    typingBudget = true;
                } else if (typingBudget) {
                    typingBudget = false;
                    typingType = true;
                }
            }

            int key = GetCharPressed();
            if (key > 0) {
                if (typingType) {
                    typeInput += static_cast<char>(key);
                } else if (typingName) {
                    nameInput += static_cast<char>(key);
                } else if (typingDescription) {
                    descriptionInput += static_cast<char>(key);
                } else if (typingBudget) {
                    budgetInput += static_cast<char>(key);
                }
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (typingType && !typeInput.empty()) {
                    typeInput.pop_back();
                } else if (typingName && !nameInput.empty()) {
                    nameInput.pop_back();
                } else if (typingDescription && !descriptionInput.empty()) {
                    descriptionInput.pop_back();
                } else if (typingBudget && !budgetInput.empty()) {
                    budgetInput.pop_back();
                }
            }

            DrawRectangleRounded({static_cast<float>(GetScreenWidth()/2.8), static_cast<float>(GetScreenHeight()/1.8), 250, 40}, 0.3f, 20, BLACK);
            DrawTextEx(*fontAll, "Enter to save", { static_cast<float>(GetScreenWidth() / 2.8 + 50), static_cast<float>(GetScreenHeight() / 1.8)}, 30  , 1,  WHITE);
            if (IsKeyPressed(KEY_ENTER) || (CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth()/2.8), static_cast<float>(GetScreenHeight()/1.8), 250, 40}) &&
                         (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) && !typeInput.empty() && !nameInput.empty() && !descriptionInput.empty() && !budgetInput.empty()) {
                category.type = typeInput;
                category.name = nameInput;
                category.description = descriptionInput;
                category.budget = std::stof(budgetInput);

                std::string csvFilePath = "data/profiles/" + account + "_profile.csv";
                std::ifstream file(csvFilePath);
                std::vector<std::string> lines;
                std::string line;
                while (std::getline(file, line)) {
                    lines.push_back(line);
                }
                file.close();

                if (category.lineNumber > 0 && category.lineNumber <= lines.size()) {
                    std::ostringstream updatedLine;
                    updatedLine << "category," << category.type << "," << category.name << "," << category.description << "," << category.budget;
                    lines[category.lineNumber - 1] = updatedLine.str();
                }

                std::ofstream outFile(csvFilePath);
                for (const auto& l : lines) {
                    outFile << l << "\n";
                }
                outFile.close();

                break;
            }

            EndDrawing();
        }
    }

        void categoryScreen(std::string account, Font *fontAll) {
        std::vector<Category> categories;

            std::string csvFilePath = "data/profiles/" + account + "_profile.csv";
            std::ifstream file(csvFilePath);
            if (file.is_open()) {
                std::string line;
                int lineNumber = 0;
                while (std::getline(file, line)) {
                    lineNumber++;
                    if (line.find("category") == 0) {
                        std::istringstream ss(line);
                        std::string type, name, description, budgetStr;
                        std::getline(ss, type, ',');
                        std::getline(ss, type, ',');
                        std::getline(ss, name, ',');
                        std::getline(ss, description, ',');
                        std::getline(ss, budgetStr, ',');
                        float budget = std::stof(budgetStr);

                        Category category;
                        category.type = type;
                        category.name = name;
                        category.description = description;
                        category.budget = budget;
                        category.lineNumber = lineNumber;

                        categories.push_back(category);
                    }
                }
                file.close();
            } else {
                std::cerr << "Failed to open file: " << csvFilePath << std::endl;
            }

        Camera2D camera = {0};
        camera.target = {0, 0};
        camera.offset = {0, 0};
        camera.zoom = 1.0f;

        float scrollSpeed = 30.0f;
        float screenHeight = static_cast<float>(GetScreenHeight());
        float categoryHeight = 80.0f;
        float baseY = 100.0f;
        float categoryAreaHeight = categories.size() * categoryHeight;

        float maxScroll = categoryAreaHeight - (screenHeight - baseY);

        bool* showNavigationsBar = new bool(false);

        if (maxScroll < 0) {
            maxScroll = 0;
        }

        while(!WindowShouldClose()) {
            if(IsKeyPressed(KEY_ESCAPE)) {
                break;
            }

            camera.target.y -= GetMouseWheelMove() * scrollSpeed;

            if (camera.target.y < 0) {
                camera.target.y = 0;
            }
            if (camera.target.y > maxScroll) {
                camera.target.y = maxScroll;
            }

            BeginDrawing();
            ClearBackground({51, 58, 63, 100});

            DrawRectangleRounded({static_cast<float>(GetScreenWidth() / 10), 20, 250, 40}, 0.3f, 20, WHITE);
            DrawTextEx(*fontAll, account.c_str(), { static_cast<float> ( GetScreenWidth()/10 + 80), 26}, 30, 1, BLACK);

            DrawRectangleRounded({static_cast<float>(GetScreenWidth() / 2.4), 20, 250, 40}, 0.3f, 20, WHITE);
            DrawTextEx(*fontAll, "New category", { static_cast<float>(GetScreenWidth()/ 2.4 + 10), 26}, 32, 1, BLACK);

            if(CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth() / 10), 20, 250, 40}) &&
                         (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
                newBudget(account, categories, fontAll);
            }

            BeginMode2D(camera);

            for(size_t i = 0; i < categories.size(); i++) {
                std::string* displayText = new std::string;
                *displayText = categories[i].type + ": " + categories[i].name + " - " + categories[i].description + " (Budget: " + std::to_string(categories[i].budget) + ")";
                DrawRectangleRounded({static_cast<float>(GetScreenWidth() / 6 + 100), static_cast<float>(GetScreenHeight() / 6 + 100 + i * 100), static_cast<float>(GetScreenWidth() / 1.8), 40},0.3f,20, WHITE);
                DrawTextEx(*fontAll, displayText->c_str(), { static_cast<float>(GetScreenWidth() / 6 + 110), static_cast<float>(GetScreenHeight() / 6 + 110 + i * 100)}, 30, 1, BLACK);
                delete displayText;
                if(CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth() / 6 + 100), static_cast<float>(GetScreenHeight() / 6 + 100 + i*100),static_cast<float>(GetScreenWidth()/1.8), 40}) &&
                         (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
                    updateCategory(account, categories[i], categories, fontAll);
                }
            }

            EndMode2D();

            DrawLine(GetScreenWidth()/16-60,GetScreenHeight()/16,GetScreenWidth()/16 - 20,GetScreenHeight()/16,WHITE);
            DrawLine(GetScreenWidth()/16-60,GetScreenHeight()/16 - 10,GetScreenWidth()/16 - 20,GetScreenHeight()/16 - 10,WHITE);
            DrawLine(GetScreenWidth()/16-60,GetScreenHeight()/16 - 20,GetScreenWidth()/16 - 20,GetScreenHeight()/16 - 20,WHITE);

            if(CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth()/16-60),static_cast<float>(GetScreenHeight()/16-20),40,20}) &&
                (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
                *showNavigationsBar = !(*showNavigationsBar);
                }

            if(*showNavigationsBar == true){
                DrawRectangle(GetScreenWidth()/48,GetScreenHeight()/7.8,150,300,GRAY);
                DrawTextEx(*fontAll, "Main screen",{static_cast<float>(GetScreenWidth() / 48 + 10),static_cast<float>(GetScreenHeight() / 7.8 + 30)}, 27, 1, WHITE);
                if(CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth()/32 + 5),static_cast<float>(GetScreenHeight()/7.8 + 30),120,20}) &&
                (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
                    break;
                }

            }

            EndDrawing();
        }

        delete showNavigationsBar;
    }

}