#include "authentication.h"
#include "accounts.h"
#include "mainScreen.h"
#include "pch.h"
namespace fs = std::filesystem;

namespace auth {

    Menu::Menu() : currentState(FormState::LOG_IN), typingUsername(true) {
        usernameInput = "";
        passwordInput = "";
        passwordMasked = "";
    }

    void Menu::resetInputs() {
        usernameInput.clear();
        passwordInput.clear();
        passwordMasked.clear();
        typingUsername = true;
    }

    void Menu::drawTextFields() {
        DrawText("Username: ", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 40, 20, LIGHTGRAY);
        DrawText(usernameInput.c_str(), GetScreenWidth() / 2 + 20, GetScreenHeight() / 2 - 40, 20, WHITE);
        DrawText("Password: ", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2+30, 20, LIGHTGRAY);
        DrawText(passwordMasked.c_str(), GetScreenWidth() / 2 + 20, GetScreenHeight() / 2+30, 20, WHITE);
    }

    void Menu::handleInput() {
        int key = GetCharPressed();
        if (key > 0) {
            if (typingUsername) {
                usernameInput += static_cast<char>(key);
            } else {
                passwordInput += static_cast<char>(key);
                passwordMasked += '*';
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (typingUsername && !usernameInput.empty()) {
                usernameInput.pop_back();
            } else if (!typingUsername && !passwordInput.empty()) {
                passwordInput.pop_back();
                passwordMasked.pop_back();
            }
        }

        if (IsKeyPressed(KEY_TAB)) {
            typingUsername = !typingUsername;
        }
    }

    void Menu::signUp() {
        DrawText("Create Account", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 100, 30, LIGHTGRAY);
        drawTextFields();

        if ((IsKeyPressed(KEY_ENTER) || (CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth() / 2 - 100), static_cast<float>(GetScreenHeight() / 2 + 100), 300, 40}) &&
            (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) && !usernameInput.empty() && !passwordInput.empty())) {

            std::string* profilePath = new std::string;
            *profilePath = "./data/profiles/" + usernameInput + "_profile.csv";

            Account* newAccount = new Account(usernameInput, passwordInput);
            newAccount->saveToFile(filePath);
            delete newAccount;

            if (fs::exists(*profilePath)) {
                DrawText("Username already exists!", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 80, 20, RED);
                resetInputs();
                return;
            }

            std::ofstream newProfile(*profilePath);
            if (newProfile.is_open()) {

                newProfile << "money,0\n";
                newProfile << "budget,0\n";
                newProfile.close();
            }

            delete profilePath;
            resetInputs();
        }
    }

    void Menu::logIn() {
        DrawText("Log In", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 100, 30, LIGHTGRAY);
        drawTextFields();

        if ((IsKeyPressed(KEY_ENTER) || (CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth() / 2 - 100), static_cast<float>(GetScreenHeight() / 2 + 100), 300, 40}) &&
            (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) && !usernameInput.empty() && !passwordInput.empty())) {

            std::ifstream file(filePath);
            std::string line, user, encryptedPass;
            bool found = false;

            std::string encryptedPasswordInput = encrypt(passwordInput, 'X');

            while (std::getline(file, line)) {
                size_t delimiterPos = line.find(",");
                user = line.substr(0, delimiterPos);
                encryptedPass = line.substr(delimiterPos + 1);

                if (user == usernameInput && encryptedPass == encryptedPasswordInput) {
                    found = true;
                    break;
                }
            }

            if (found) {
                DrawText("Login Successful!", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 80, 20, GREEN);
                mainScreen::mainScreen(usernameInput);
            } else {
                DrawText("Invalid Credentials!", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 80, 20, RED);
            }

            resetInputs();
            }
    }

    void Menu::mainMenu() {
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "CryptoFi");
    SetTargetFPS(60);
    SetExitKey(KEY_ESCAPE);

    Rectangle logInRec = { static_cast<float>(GetScreenWidth() / 2 - 200), static_cast<float>(GetScreenHeight() / 8 + 100), 200, 40 };
    Rectangle signUpRec = { static_cast<float>(GetScreenWidth() / 2 + 200), static_cast<float>(GetScreenHeight() / 8 + 100), 200, 40 };

    //Vector2 shadowOffset = ;

    Rectangle* usernameRecPtr = new Rectangle{ static_cast<float>(GetScreenWidth() / 2 - 120), static_cast<float>(GetScreenHeight() / 2 - 50), 350, 50 };
    Rectangle* passwordRecPtr = new Rectangle{ static_cast<float>(GetScreenWidth() / 2 - 120), static_cast<float>(GetScreenHeight() / 2 + 20), 350, 50 };

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground({51, 58, 63, 100});

        DrawText("Welcome to CryptoFi", GetScreenWidth() / 2 - 100, GetScreenHeight() / 8, 30, WHITE);

        DrawRectangleRec(logInRec, BLUE);
        DrawRectangleRec(signUpRec, BLUE);

        DrawRectangleRounded({ usernameRecPtr->x + 10, usernameRecPtr->y + 10, usernameRecPtr->width, usernameRecPtr->height }, 0.3f, 20, {0, 0, 0, 100});
        DrawRectangleRounded({ passwordRecPtr->x + 10, passwordRecPtr->y + 10, passwordRecPtr->width, passwordRecPtr->height }, 0.3f, 20, {0, 0, 0, 100});

        DrawRectangleRounded(*usernameRecPtr, 0.3f, 20, BLACK);
        DrawRectangleRounded(*passwordRecPtr, 0.3f, 20, BLACK);

        DrawRectangleRounded({ static_cast<float>(GetScreenWidth() / 2 - 120), static_cast<float>(GetScreenHeight() / 2 + 100), 350, 40 }, 0.3f, 20, WHITE);
        DrawText("SUBMIT", GetScreenWidth() / 2 + 10, GetScreenHeight() / 2 + 110, 20, BLACK);

        if (CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth() / 2 - 100), static_cast<float>(GetScreenHeight() / 2 - 50), 300, 40}) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
            typingUsername = true;
        } else if (CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth() / 2 - 100), static_cast<float>(GetScreenHeight() / 2)+30, 300, 40}) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
            typingUsername = false;
        }

        if (CheckCollisionPointRec(GetMousePosition(), logInRec) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
            currentState = FormState::LOG_IN;
        }

        if (CheckCollisionPointRec(GetMousePosition(), signUpRec) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
            currentState = FormState::SIGN_UP;
        }

        if (currentState == FormState::SIGN_UP) {
            signUp();
        } else if (currentState == FormState::LOG_IN) {
            logIn();
        }

        handleInput();
        EndDrawing();
    }

    delete usernameRecPtr;
    delete passwordRecPtr;

    CloseWindow();
}


}