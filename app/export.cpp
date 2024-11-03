#include "export.h"

namespace export_ns {

    std::chrono::system_clock::time_point parseDateWithTime(const std::string& dateStr, bool isFromDate) {
        std::tm tm = {};
        std::istringstream ss(dateStr + (isFromDate ? " 00:00:00" : " 23:59:59"));
        ss >> std::get_time(&tm, "%d/%m/%Y %H:%M:%S");
        if (ss.fail()) {
            std::cerr << "Failed to parse date: " << dateStr << std::endl;
            return std::chrono::system_clock::now();
        }
        std::time_t time = std::mktime(&tm);
        return std::chrono::system_clock::from_time_t(time);
    }

    void exportScreen(std::vector<mainScreen::Transaction*> transactions,  std::string& account, float* money) {
        std::string fromDateInput;
        std::string toDateInput;
        bool typingFromDate = true;
        bool* showNavigationsBar = new bool(false);
        while (!WindowShouldClose()) {
            BeginDrawing();

            ClearBackground({51, 58, 63, 100});

            DrawRectangleRounded({static_cast<float>(GetScreenWidth()/10), 20, 250, 40}, 0.3f, 20, WHITE);
            DrawText(account.c_str(), GetScreenWidth()/10+80, 30, 25, BLACK);

            DrawRectangleRounded({static_cast<float>(GetScreenWidth()/2.4), 20, 250, 40}, 0.3f, 20, WHITE);
            DrawText(TextFormat("Money: %.2f", *money), GetScreenWidth()/2.4+10, 30, 20, GRAY);

            DrawRectangleRounded({static_cast<float>(GetScreenWidth()/1.4),20,250,40},0.3f,20,WHITE);
            DrawText("Export",GetScreenWidth()/1.3, 30, 25, BLACK);

            DrawRectangleRoundedLines({static_cast<float>(GetScreenWidth() / 2 - 210), static_cast<float>(GetScreenHeight() / 2 - 130), 420, 260}, 0.3f, 20,BLACK);
            DrawRectangleRounded({static_cast<float>(GetScreenWidth() / 2 - 210), static_cast<float>(GetScreenHeight() / 2 - 130), 420, 260}, 0.3f, 20, WHITE);

            DrawRectangleRounded({static_cast<float>(GetScreenWidth() / 2 - 140), static_cast<float>(GetScreenHeight() / 2 -75), 270, 30},0.3f, 20, BLACK);
            DrawText("FORMAT DD/MM/YYYY", GetScreenWidth() / 2 - 120, GetScreenHeight() / 2 -70, 20, WHITE);

            DrawText("From:", GetScreenWidth() / 2 - 150, GetScreenHeight() / 2 - 30, 20, BLACK);
            DrawText(fromDateInput.c_str(), GetScreenWidth() / 2 - 80, GetScreenHeight() / 2 - 30, 20, BLACK);

            DrawText("To:", GetScreenWidth() / 2 - 150, GetScreenHeight() / 2, 20, BLACK);
            DrawText(toDateInput.c_str(), GetScreenWidth() / 2 - 100, GetScreenHeight() / 2, 20, BLACK);

            DrawRectangleRounded({static_cast<float>(GetScreenWidth() / 2 - 40), static_cast<float>(GetScreenHeight() / 2 + 50), 120, 30},0.3f, 20, BLACK);
            DrawText("Export", GetScreenWidth() / 2 - 20, GetScreenHeight() / 2 + 55, 20, WHITE);

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

            if (IsKeyPressed(KEY_TAB)) {
                typingFromDate = !typingFromDate;
            }

            int key = GetCharPressed();
            if (key > 0) {
                if (typingFromDate) {
                    fromDateInput += static_cast<char>(key);
                } else {
                    toDateInput += static_cast<char>(key);
                }
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (typingFromDate && !fromDateInput.empty()) {
                    fromDateInput.pop_back();
                } else if (!typingFromDate && !toDateInput.empty()) {
                    toDateInput.pop_back();
                }
            }

            if (CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth() / 2 - 30), static_cast<float>(GetScreenHeight() / 2 + 50), 90, 20}) &&
                (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {

                std::filesystem::create_directory("exports");

                xlnt::workbook wb;
                xlnt::worksheet ws = wb.active_sheet();
                ws.title("Transactions");

                ws.cell("A1").value("Type");
                ws.cell("B1").value("Reason");
                ws.cell("C1").value("Date");
                ws.cell("D1").value("Difference");

                auto fromDate = parseDateWithTime(fromDateInput, true);
                auto toDate = parseDateWithTime(toDateInput, false);

                int row = 2;
                for (const auto& transaction : transactions) {
                    if (transaction->date >= fromDate && transaction->date <= toDate) {
                        ws.cell("A" + std::to_string(row)).value(transaction->type);
                        ws.cell("B" + std::to_string(row)).value(transaction->reason);
                        ws.cell("C" + std::to_string(row)).value(mainScreen::formatDate(transaction->date));
                        ws.cell("D" + std::to_string(row)).value(transaction->difference);
                        row++;
                    }
                }

                auto now = std::chrono::system_clock::now();
                std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
                std::tm* tm = std::localtime(&nowTime);
                std::ostringstream oss;
                oss << std::put_time(tm, "%Y%m%d_%H%M%S");

                std::string filename = "exports/" + account + "_export_" + oss.str() + ".xlsx";
                wb.save(filename);

                HPDF_Doc pdf = HPDF_New(nullptr, nullptr);
                if (!pdf) {
                    std::cerr << "Failed to create PDF document" << std::endl;
                    continue;
                }

                HPDF_Page page = HPDF_AddPage(pdf);
                HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

                HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", nullptr);
                HPDF_Page_SetFontAndSize(page, font, 12);

                float y = 750;
                HPDF_Page_BeginText(page);
                HPDF_Page_TextOut(page, 50, y, "Type");
                HPDF_Page_TextOut(page, 150, y, "Reason");
                HPDF_Page_TextOut(page, 300, y, "Date");
                HPDF_Page_TextOut(page, 450, y, "Difference");
                HPDF_Page_EndText(page);

                y -= 20;
                for (const auto& transaction : transactions) {
                    if (transaction->date >= fromDate && transaction->date <= toDate) {
                        HPDF_Page_BeginText(page);
                        HPDF_Page_TextOut(page, 50, y, transaction->type.c_str());
                        HPDF_Page_TextOut(page, 150, y, transaction->reason.c_str());
                        HPDF_Page_TextOut(page, 300, y, mainScreen::formatDate(transaction->date).c_str());
                        HPDF_Page_TextOut(page, 450, y, std::to_string(transaction->difference).c_str());
                        HPDF_Page_EndText(page);
                        y -= 20;
                    }
                }

                std::string pdfFilename = "exports/" + account + "_export_" + oss.str() + ".pdf";
                HPDF_SaveToFile(pdf, pdfFilename.c_str());
                HPDF_Free(pdf);
                break;
            }

            EndDrawing();
        }
        delete showNavigationsBar;
    }
}