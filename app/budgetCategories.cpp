#include "budgetCategories.h"
#include "accounts.h"

    namespace budgetCategories {

        void printCategory(Category category, int posYmod) {
            DrawRectangle(GetScreenWidth() / 4 + 200, GetScreenHeight() / 2 + 100 - posYmod, 200, 80, GRAY);
            DrawText(category.name.c_str(), GetScreenWidth() / 4 + 210, GetScreenHeight() / 2 + 90 - posYmod, 20, WHITE);
        }

        void setDefaults(std::vector<Category> &def) {
            for(int i = 0; i < 5; i++) {
                switch(i) {
                    case 0: {
                        def[i].name = "Foodstuff";
                        def[i].type = "Physiological";
                        def[i].description = "Everything edible. Drinks included.";
                        break;
                    }
                    case 1: {
                        def[i].name = "Electronics";
                        def[i].type = "Technology";
                        def[i].description = "Everything that uses electricity.";
                        break;
                    }
                    case 2: {
                        def[i].name = "Hardware";
                        def[i].type = "Technology";
                        def[i].description = "Everything you buy at a hardware store.";
                        break;
                    }
                    case 3: {
                        def[i].name = "Furniture";
                        def[i].type = "Technology";
                        def[i].description = "Objects such as tables, chairs, beds, etc.";
                        break;
                    }
                    case 4: {
                        def[i].name = "Clothing";
                        def[i].type = "Physiological";
                        def[i].description = "The things you wear duh.";
                        break;
                    }
                }
            }
        }

        void categoryScreen(std::string account) {
            std::vector<Category> allCategories;
            setDefaults(allCategories);

            while(!WindowShouldClose()) {
                if(IsKeyPressed(KEY_ESCAPE)) {
                    break;
                }

                BeginDrawing();
                ClearBackground(RAYWHITE);

                int totalCategories = 3;

                DrawText("Select a category to set a budget for:", GetScreenWidth() / 4 - 100, GetScreenHeight() / 2 + 100, 30, LIGHTGRAY);

                for(int i = 0; i < totalCategories; i++) {
                    printCategory(allCategories[i], 80*i);

                }

                EndDrawing();
            }
        }
    }