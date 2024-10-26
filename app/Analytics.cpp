#include "Analytics.h"

namespace analytics {
    void analytics(std::vector<mainScreen::Transaction*> transactions) {
        while(!WindowShouldClose()) {
            int* posX = new int(GetScreenWidth()/16+10);
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawLine(GetScreenWidth()/16,GetScreenHeight()/8,GetScreenWidth()/16,GetScreenHeight()/8+200,GRAY);
            DrawLine(GetScreenWidth()/16,GetScreenHeight()/4,GetScreenWidth()/2,GetScreenHeight()/4,GRAY);

            for(int i = 0; i < transactions.size(); i++) {
                if(transactions[i]->type == "revenue") {
                    DrawRectangle(*posX,(GetScreenHeight()/4)-transactions[i]->difference*(transactions[i]->difference<10?2:0.4),10,transactions[i]->difference*(transactions[i]->difference<10?2:0.4),GREEN);
                }
                if(transactions[i]->type == "expense") {
                    DrawRectangle(*posX,GetScreenHeight()/4,10,abs(transactions[i]->difference)*(transactions[i]->difference<10?2:0.4),RED);
                }
                *posX+=10;
            }
            EndDrawing();
            delete posX;
        }

    }

}
