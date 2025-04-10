#ifndef LOGIN_UI_H
#define LOGIN_UI_H

#include "UIComponent.h"
#include "UserManager.h"
#include "UserSession.h"
#include <iostream>

class LoginUI : public UIComponent {
private:
    UserManager* userManager;

public:
    LoginUI(UserManager* um) : userManager(um) {}

    void display() override {
        std::string email, password;
        std::cout << "=== Login ===\n";
        std::cout << "Email: ";
        std::cin >> email;
        std::cout << "Password: ";
        std::cin >> password;

        if (userManager->authenticateUser(email, password)) {
            UserSession::getInstance()->login(email);
            std::cout << "Login successful!\n";
            if (mediator) mediator->notify("LoginUI", "login_success");
        } else {
            std::cout << "Invalid credentials.\n";
        }
    }
};

#endif