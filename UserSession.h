#ifndef USERSESSION_H
#define USERSESSION_H

#include <string>

class UserSession {
private:
    std::string loggedInEmail;
    static UserSession* instance;

    UserSession() {}

public:
    UserSession(const UserSession&) = delete;
    UserSession& operator=(const UserSession&) = delete;

    static UserSession* getInstance();

    void login(const std::string& email);
    void logout();
    bool isLoggedIn() const;
    std::string getLoggedInEmail() const;
};

#endif