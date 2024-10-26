#include "budgetCategories.h"
#include "accounts.h"

    namespace budgetCategories {

        void setDefaults(std::vector<Category> &def) {
            for(int i = 0; i < 9; i++) {
                Category *temp = new Category;
                switch(i) {
                    case 0: {
                        temp->name = "Foodstuff";
                        temp->type = "Physiological";
                        temp->description = "Everything edible. Drinks included.";
                        break;
                    }
                    case 1: {
                        temp->name = "Clothes";
                        temp->type = "Physiological";
                        temp->description = "Everything wearable.";
                        break;
                    }
                    case 2: {
                        temp->name = "Electronics";
                        temp->type = "Technological";
                        temp->description = "Everything that requires electricity to run.";
                        break;
                    }
                    case 3: {
                        temp->name = "Furniture";
                        temp->type = "Technological";
                        temp->description = "Everything from beds to cupboards is considered furniture.";
                        break;
                    }
                    case 4: {
                        temp->name = "A new chess set";
                        temp->type = "Entertainment and Leisure";
                        temp->description = "Your old one broke, so I guess you need a new one.";
                        break;
                    }
                    case 5: {
                        temp->name = "A new chess set";
                        temp->type = "Entertainment and Leisure";
                        temp->description = "Your old one broke, so I guess you need a new one.";
                        break;
                    }
                    case 6: {
                        temp->name = "A new chess set";
                        temp->type = "Entertainment and Leisure";
                        temp->description = "Your old one broke, so I guess you need a new one.";
                        break;
                    }
                    case 7: {
                        temp->name = "A new chess set";
                        temp->type = "Entertainment and Leisure";
                        temp->description = "Your old one broke, so I guess you need a new one.";
                        break;
                    }
                    case 8: {
                        temp->name = "A new chess set";
                        temp->type = "Entertainment and Leisure";
                        temp->description = "Your old one broke, so I guess you need a new one.";
                        break;
                    }
                }
                def.push_back(*temp);
                delete temp;
            }
        }

        void categoryScreen(std::string account) {
            std::vector<Category> allCategories;
            setDefaults(allCategories);

            Camera2D camera = {0};
            camera.target = {0, 0};
            camera.offset = {0, 0};
            camera.zoom = 1.0f;

            float scrollSpeed = 30.0f;
            float screenHeight = static_cast<float>(GetScreenHeight());
            float categoryHeight = 80.0f;
            float baseY = 100.0f;
            float categoryAreaHeight = allCategories.size() * categoryHeight;

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
                ClearBackground(RAYWHITE);

                DrawText("Select a category to set a budget for:", GetScreenWidth() / 6 - 100, GetScreenHeight() / 6, 30, LIGHTGRAY);

                BeginMode2D(camera);

                for(size_t i = 0; i < allCategories.size(); i++) {
                    DrawRectangle(GetScreenWidth() / 6 + 100, GetScreenHeight() / 6 + 100 + i*80, 200, 80, GRAY);
                    DrawText(allCategories[i].name.c_str(), GetScreenWidth() / 6 + 110, GetScreenHeight() / 6 + 110 + i*80, 20, BLACK);
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

                    DrawText("Analytics",GetScreenWidth()/1.15+5,GetScreenHeight()/7.8+40,20,WHITE);

                    if(CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth()/1.15+5),static_cast<float>(GetScreenHeight()/7.8+40),40,20}) &&
                    (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
                        std::cout << "Analytics" << std::endl;
                    }
                }

                EndDrawing();
            }
        }
    }