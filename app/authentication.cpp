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

    void Menu::drawTextFields(Font *fontAll) {
        DrawTextEx(*fontAll, "Username: ", { static_cast<float>(GetScreenWidth() / 2 - 100), static_cast<float>(GetScreenHeight() / 2 - 20) }, 31, 1, LIGHTGRAY);
        DrawTextEx(*fontAll ,usernameInput.c_str(), { static_cast<float>(GetScreenWidth() / 2 + 30), static_cast<float>(GetScreenHeight() / 2 - 20) }, 31, 1, WHITE);
        DrawTextEx(*fontAll, "Password: ", { static_cast<float>(GetScreenWidth() / 2 - 100 ), static_cast<float>(GetScreenHeight() / 2 + 50) }, 31, 1, LIGHTGRAY);
        DrawTextEx(*fontAll, passwordMasked.c_str(), {static_cast<float>(GetScreenWidth() / 2 + 30), static_cast<float>(GetScreenHeight() / 2 + 50) }, 45, 1, WHITE);
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

    void Menu::signUp(Font *fontAll) {
        DrawTextEx(*fontAll, "Create Account:", { static_cast<float> ( GetScreenWidth() / 2 - 110 ),static_cast<float> ( GetScreenHeight() / 2 - 80 ) }, 35, 1, LIGHTGRAY);
        drawTextFields(fontAll);

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

    void Menu::logIn(Font *fontAll) {
        DrawTextEx(*fontAll, "Log in:", {static_cast<float> (GetScreenWidth() / 2 - 110),static_cast<float> (GetScreenHeight() / 2 - 80 )}, 35, 1, LIGHTGRAY);
        drawTextFields(fontAll);

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
                DrawTextEx(*fontAll,"Login Successful!", {static_cast<float>(GetScreenWidth() / 2 - 120), static_cast<float>(GetScreenHeight() / 2 + 90)}, 30, 1, GREEN);
                mainScreen::mainScreen(usernameInput, fontAll);
            } else {
                DrawTextEx(*fontAll, "Invalid Credentials!", {static_cast<float>(GetScreenWidth() / 2 - 120), static_cast<float>(GetScreenHeight() / 2 + 90)}, 30, 1,RED);
            }

            resetInputs();
            }
    }

    void Menu::mainMenu() {
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "CryptoFi");
    SetTargetFPS(60);
    SetExitKey(KEY_ESCAPE);
        SetWindowIcon(LoadImage("resources/images/logo.png"));

    Texture2D logo = LoadTexture("resources/images/logo.png");

    Font font = LoadFont("resources/font/Manrope-Bold.ttf");
    Font* fontAll = new Font(LoadFont("resources/font/Manrope-Bold.ttf"));

    Rectangle logInRec = { static_cast<float>(GetScreenWidth() / 2 - 200), static_cast<float>(GetScreenHeight() / 8 + 135), 200, 40 };
    Rectangle signUpRec = { static_cast<float>(GetScreenWidth() / 2 + 200), static_cast<float>(GetScreenHeight() / 8 + 135), 200, 40 };

    //Vector2 shadowOffset = ;

    Rectangle* usernameRecPtr = new Rectangle{ static_cast<float>(GetScreenWidth() / 2 - 120), static_cast<float>(GetScreenHeight() / 2 - 30), 350, 50 };
    Rectangle* passwordRecPtr = new Rectangle{ static_cast<float>(GetScreenWidth() / 2 - 120), static_cast<float>(GetScreenHeight() / 2 + 40), 350, 50 };

    while (!WindowShouldClose()) {
        logInRec = { static_cast<float>(GetScreenWidth() / 2 - 200), static_cast<float>(GetScreenHeight() / 8 + 135), 200, 40 };
        signUpRec = { static_cast<float>(GetScreenWidth() / 2 + 200), static_cast<float>(GetScreenHeight() / 8 + 135), 200, 40 };

        *usernameRecPtr = { static_cast<float>(GetScreenWidth() / 2 - 120), static_cast<float>(GetScreenHeight() / 2 - 30), 350, 50 };
        *passwordRecPtr =   { static_cast<float>(GetScreenWidth() / 2 - 120), static_cast<float>(GetScreenHeight() / 2 + 40), 350, 50 };
        BeginDrawing();
        ClearBackground({51, 58, 63, 100});

        DrawTextureEx(logo, {640, 25}, 0, 0.35, WHITE);
        DrawTextEx(*fontAll, "Welcome to CryptoFi", { static_cast<float>(GetScreenWidth() / 2 - 100 ),static_cast<float>( GetScreenHeight() / 4.6 )}, 50, 1, WHITE);

        DrawRectangleRec(logInRec, BLACK);
        DrawTextEx(*fontAll, "Log in" , { static_cast<float>(logInRec.x + 20), static_cast<float>(logInRec.y + 5) }, 30, 1, WHITE);
        DrawRectangleRec(signUpRec, BLACK);
        DrawTextEx(*fontAll, "Sign up", { static_cast<float>(signUpRec.x + 20), static_cast<float>(signUpRec.y + 5) }, 30, 1, WHITE);

        DrawRectangleRounded({ usernameRecPtr->x + 10, usernameRecPtr->y + 10, usernameRecPtr->width, usernameRecPtr->height }, 0.3f, 20, {0, 0, 0, 100});
        DrawRectangleRounded({ passwordRecPtr->x + 10, passwordRecPtr->y + 10, passwordRecPtr->width, passwordRecPtr->height }, 0.3f, 20, {0, 0, 0, 100});

        DrawRectangleRounded(*usernameRecPtr, 0.3f, 20, BLACK);
        DrawRectangleRounded(*passwordRecPtr, 0.3f, 20, BLACK);

        DrawRectangleRounded({ static_cast<float>(GetScreenWidth() / 2 - 120), static_cast<float>(GetScreenHeight() / 2 + 125), 350, 40 }, 0.3f, 20, WHITE);
        DrawTextEx(*fontAll, "Continue", {static_cast<float> (GetScreenWidth() / 2 + 10 ), static_cast<float> (GetScreenHeight() / 2 + 128 ) }, 32, 1, BLACK);

        if (CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth() / 2 - 100), static_cast<float>(GetScreenHeight() / 2 - 50), 300, 40}) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
            typingUsername = true;
        } else if (CheckCollisionPointRec(GetMousePosition(), {static_cast<float>(GetScreenWidth() / 2 - 100), static_cast<float>(GetScreenHeight() / 2) + 30, 300, 40}) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
            typingUsername = false;
        }

        if (CheckCollisionPointRec(GetMousePosition(), logInRec) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
            currentState = FormState::LOG_IN;
        }

        if (CheckCollisionPointRec(GetMousePosition(), signUpRec) && (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))) {
            currentState = FormState::SIGN_UP;
        }

        if (currentState == FormState::SIGN_UP) {
            signUp(fontAll);
        } else if (currentState == FormState::LOG_IN) {
            logIn(fontAll);
        }

        handleInput();
        EndDrawing();
    }

    delete usernameRecPtr;
    delete passwordRecPtr;
    delete fontAll;

    CloseWindow();
}


}