#include "UserSession.h"

UserSession* UserSession::instance = nullptr;

UserSession* UserSession::getInstance() {
    if (instance == nullptr) {
        instance = new UserSession();
    }
    return instance;
}

void UserSession::login(const std::string& email) {
    loggedInEmail = email;
}

void UserSession::logout() {
    loggedInEmail.clear();
}

bool UserSession::isLoggedIn() const {
    return !loggedInEmail.empty();
}

std::string UserSession::getLoggedInEmail() const {
    return loggedInEmail;
}