#include "Analytics.h"
#include "predictions.h"

namespace analytics {
    void analytics(std::vector<mainScreen::Transaction*> transactions, std::string& account, float* money) {
    int* revenueCount = new int(0);
    int* expenseCount = new int(0);

    for (int i = 0; i < transactions.size(); i++) {
        if (transactions[i]->type == "revenue") {
            (*revenueCount)++;
        }
        if (transactions[i]->type == "expense") {
            (*expenseCount)++;
        }
    }

    int* totalTransactions = new int(*revenueCount + *expenseCount);

        prediction::TransactionPredictor predictor;
        predictor.analyzeTransactions(transactions);
        std::string prediction = predictor.predictNextTransaction(transactions);

        bool* showNavigationsBar = new bool(false);

    while (!WindowShouldClose()) {
        int* posX = new int(GetScreenWidth() / 8 + 10);

        BeginDrawing();
        ClearBackground({51, 58, 63, 100});

        DrawRectangleRounded({static_cast<float>(GetScreenWidth()/10), 20, 250, 40}, 0.3f, 20, WHITE);
        DrawText(account.c_str(), GetScreenWidth()/10+80, 30, 25, BLACK);

        DrawRectangleRounded({static_cast<float>(GetScreenWidth()/2.4), 20, 250, 40}, 0.3f, 20, WHITE);
        DrawText(TextFormat("Money: %.2f", *money), GetScreenWidth()/2.4+10, 30, 20, GRAY);

        DrawRectangleRounded({static_cast<float>(GetScreenWidth()/16-20), static_cast<float>(GetScreenHeight()/1.2-10), static_cast<float>(GetScreenWidth()/1.2), 40}, 0.3f, 20, WHITE);
        DrawText(prediction.c_str(), GetScreenWidth() / 16, GetScreenHeight() / 1.2, 20, BLACK);

        DrawLine(GetScreenWidth() / 16, GetScreenHeight() / 3, GetScreenWidth() / 16, GetScreenHeight() / 3+200, WHITE);
        DrawLine(GetScreenWidth() / 16, GetScreenHeight() / 2, GetScreenWidth() / 2, GetScreenHeight() / 2, WHITE);

        for (int i = 0; i < transactions.size(); i++) {
            float* height = new float;
            if (transactions[i]->difference < 10) {
                *height = transactions[i]->difference * 5;
            } else {
                *height = log2(transactions[i]->difference + 1) * 15;
            }

            *height = fmax(*height, 5);

            if (transactions[i]->type == "revenue") {
                DrawRectangle(*posX, (GetScreenHeight() / 2) - *height, 10, *height, GREEN);
            } else if (transactions[i]->type == "expense") {
                DrawRectangle(*posX, GetScreenHeight() / 2, 10, *height, RED);
            }

            *posX += 10;
            delete height;
        }

        DrawRectangle(GetScreenWidth()-300,GetScreenHeight()/2,20,20,GREEN);
        DrawRectangle(GetScreenWidth()-300,GetScreenHeight()/1.8,20,20,RED);

        DrawText("Revenue",GetScreenWidth()-270,GetScreenHeight()/2,20,WHITE);
        DrawText("Expense",GetScreenWidth()-270,GetScreenHeight()/1.8,20,WHITE);

        int* pieCenterX = new int(GetScreenWidth() - 150);
        int* pieCenterY = new int(GetScreenHeight() / 2);
        int* pieRadius = new int(80);

        if (*totalTransactions > 0) {
            float* revenueAngle = new float((*revenueCount / static_cast<float>(*totalTransactions)) * 360.0f);
            float* expenseAngle = new float((*expenseCount / static_cast<float>(*totalTransactions)) * 360.0f);

            DrawCircleSector({static_cast<float>(*pieCenterX)-50, static_cast<float>(*pieCenterY)-100}, static_cast<float>(*pieRadius), 0, *revenueAngle, 100, GREEN);
            DrawCircleSector({static_cast<float>(*pieCenterX)-50, static_cast<float>(*pieCenterY)-100}, static_cast<float>(*pieRadius), *revenueAngle, *revenueAngle + *expenseAngle, 100, RED);

            delete revenueAngle;
            delete expenseAngle;
        }

        DrawLine(GetScreenWidth()/16-60,GetScreenHeight()/16,GetScreenWidth()/16-20,GetScreenHeight()/16,WHITE);
        DrawLine(GetScreenWidth()/16-60,GetScreenHeight()/16-10,GetScreenWidth()/16-20,GetScreenHeight()/16-10,WHITE);
        DrawLine(GetScreenWidth()/16-60,GetScreenHeight()/16-20,GetScreenWidth()/16-20,GetScreenHeight()/16-20,WHITE);

        if(CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth()/16-60),static_cast<float>(GetScreenHeight()/16-20),40,20}) &&
            (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
            *showNavigationsBar = !(*showNavigationsBar);
            }

        if(*showNavigationsBar == true){
            DrawRectangle(GetScreenWidth()/48,GetScreenHeight()/7.8,150,300,GRAY);
            DrawText("Main screen",GetScreenWidth()/48+10,GetScreenHeight()/7.8+20,20,WHITE);
            if(CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth()/32+5),static_cast<float>(GetScreenHeight()/7.8+20),120,20}) &&
            (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
                break;
            }

        }

        delete pieCenterX;
        delete pieCenterY;
        delete pieRadius;

        EndDrawing();
        delete posX;
    }

    delete revenueCount;
    delete expenseCount;
    delete totalTransactions;
}


}
