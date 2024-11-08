#pragma once
#include "pch.h"

namespace auth {

    class Menu {
    public:
        Menu();

        void resetInputs();
        void drawTextFields(Font *fontAll);
        void handleInput();
        void signUp(Font *fontAll);
        void logIn(Font *fontAll);
        void mainMenu();

    private:
        enum class FormState { SIGN_UP, LOG_IN };
        FormState currentState;

        std::string usernameInput;
        std::string passwordInput;
        std::string passwordMasked;
        bool typingUsername;
        const std::string filePath = "data/accounts.csv";
    };

}
