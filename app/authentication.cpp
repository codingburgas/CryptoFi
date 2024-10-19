#include "authentication.h"
#include "accounts.h"
#include "mainScreen.h"
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
        DrawText(usernameInput.c_str(), GetScreenWidth() / 2 + 20, GetScreenHeight() / 2 - 40, 20, DARKGRAY);
        DrawText("Password: ", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2, 20, LIGHTGRAY);
        DrawText(passwordMasked.c_str(), GetScreenWidth() / 2 + 20, GetScreenHeight() / 2, 20, DARKGRAY);
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
        DrawText("Create Account", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 80, 30, LIGHTGRAY);
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
                newProfile.close();
            }
            delete profilePath;
            resetInputs();
        }
    }

    void Menu::logIn() {
        DrawText("Log In", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 80, 30, LIGHTGRAY);
        drawTextFields();

        if ((IsKeyPressed(KEY_ENTER) || (CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth() / 2 - 100), static_cast<float>(GetScreenHeight() / 2 + 100), 300, 40}) &&
            (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) && !usernameInput.empty() && !passwordInput.empty())) {

            std::ifstream file(filePath);
            std::string line, user, pass;
            bool found = false;

            while (std::getline(file, line)) {
                size_t delimiterPos = line.find(",");
                user = line.substr(0, delimiterPos);
                pass = line.substr(delimiterPos + 1);

                if (user == usernameInput && pass == passwordInput) {
                    found = true;
                    break;
                }
            }

            if (found) {
                DrawText("Login Successful!", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 80, 20, GREEN);
                mainScreen::mainScreen();
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

        Rectangle logInRec = { static_cast<float>(GetScreenWidth() / 2 - 200), static_cast<float>(GetScreenHeight() / 8 + 100), 200, 40 };
        Rectangle signUpRec = { static_cast<float>(GetScreenWidth() / 2 + 200), static_cast<float>(GetScreenHeight() / 8 + 100), 200, 40 };

        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangleRec(logInRec, BLUE);
            DrawRectangleRec(signUpRec, BLUE);

            DrawRectangle(GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 100, 300, 40, BLACK);

            if (CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth() / 2 - 100), static_cast<float>(GetScreenHeight() / 2 - 50), 300, 40}) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
                typingUsername = true;
            } else if (CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth() / 2 - 100), static_cast<float>(GetScreenHeight() / 2), 300, 40}) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
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

        CloseWindow();
    }

}
