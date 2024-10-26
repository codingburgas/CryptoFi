#include "Analytics.h"

namespace analytics {
    void analytics(std::vector<mainScreen::Transaction*> transactions) {
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

    while (!WindowShouldClose()) {
        int* posX = new int(GetScreenWidth() / 16 + 10);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawLine(GetScreenWidth() / 16, GetScreenHeight() / 8, GetScreenWidth() / 16, GetScreenHeight() / 8 + 200, GRAY);
        DrawLine(GetScreenWidth() / 16, GetScreenHeight() / 4, GetScreenWidth() / 2, GetScreenHeight() / 4, GRAY);

        for (int i = 0; i < transactions.size(); i++) {
            if (transactions[i]->type == "revenue") {
                DrawRectangle(*posX, (GetScreenHeight() / 4) - transactions[i]->difference * (transactions[i]->difference < 10 ? 2 : 0.4), 10, transactions[i]->difference * (transactions[i]->difference < 10 ? 2 : 0.4), GREEN);
            }
            if (transactions[i]->type == "expense") {
                DrawRectangle(*posX, GetScreenHeight() / 4, 10, abs(transactions[i]->difference) * (transactions[i]->difference < 10 ? 2 : 0.4), RED);
            }
            *posX += 10;
        }

        DrawRectangle(GetScreenWidth()-300,GetScreenHeight()/16,20,20,GREEN);
        DrawRectangle(GetScreenWidth()-300,GetScreenHeight()/8,20,20,RED);

        DrawText("Revenue",GetScreenWidth()-270,GetScreenHeight()/16,20,GRAY);
        DrawText("Expense",GetScreenWidth()-270,GetScreenHeight()/8,20,GRAY);

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
