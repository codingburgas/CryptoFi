#include "budgetCategories.h"
#include "mainScreen.h"

namespace budgetCategories {

        void newBudget(std::string account, std::vector<Category>& categories) {
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
            ClearBackground(RAYWHITE);

            DrawText("Enter Category Type:", 100, 100, 20, BLACK);
            DrawText(typeInput.c_str(), 300, 100, 20, DARKGRAY);

            DrawText("Enter Category Name:", 100, 150, 20, BLACK);
            DrawText(nameInput.c_str(), 300, 150, 20, DARKGRAY);

            DrawText("Enter Category Description:", 100, 200, 20, BLACK);
            DrawText(descriptionInput.c_str(), 300, 200, 20, DARKGRAY);

            DrawText("Enter Category Budget:", 100, 250, 20, BLACK);
            DrawText(budgetInput.c_str(), 300, 250, 20, DARKGRAY);

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

            if (IsKeyPressed(KEY_ENTER) && !typeInput.empty() && !nameInput.empty() && !descriptionInput.empty() && !budgetInput.empty()) {
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

        void categoryScreen(std::string account) {
        std::vector<Category> categories;

        std::string csvFilePath = "data/profiles/" + account + "_profile.csv";
        std::ifstream file(csvFilePath);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                if (line.find("category") == 0) {
                    std::istringstream ss(line);
                    std::string type, name, description;
                    float budget;
                    std::getline(ss, type, ',');
                    std::getline(ss, name, ',');
                    std::getline(ss, description, ',');
                    ss >> budget;

                    Category category;
                    category.type = type;
                    category.name = name;
                    category.description = description;
                    category.budget = budget;

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

            DrawRectangleRounded({static_cast<float>(GetScreenWidth()/10), 20, 250, 40}, 0.3f, 20, WHITE);
            DrawText("New category", GetScreenWidth()/10+80, 30, 25, BLACK);

            if(CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth()/10), 20, 250, 40}) &&
                         (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
                newBudget(account, categories);
            }

            BeginMode2D(camera);

            for(size_t i = 0; i < categories.size(); i++) {
                std::string* displayText = new std::string;
                *displayText = categories[i].type + ": " + categories[i].name + " - " + categories[i].description + " (Budget: " + std::to_string(categories[i].budget) + ")";
                DrawRectangle(GetScreenWidth() / 6 + 100, GetScreenHeight() / 6 + 100 + i*80, GetScreenWidth()/1.8, 80, GRAY);
                DrawText(displayText->c_str(), GetScreenWidth() / 6 + 110, GetScreenHeight() / 6 + 110 + i*80, 20, BLACK);
                delete displayText;
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
                DrawText("Main Screen",GetScreenWidth()/1.15+5,GetScreenHeight()/7.8+20,20,WHITE);
                if(CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth()/1.15+5),static_cast<float>(GetScreenHeight()/7.8+20),40,20}) &&
                (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
                    break;
                }
            }

            EndDrawing();
        }

        delete showNavigationsBar;
    }

    }