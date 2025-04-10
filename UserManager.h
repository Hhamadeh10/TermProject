#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "User.h"
#include <sqlite3.h>
#include <string>
#include <optional>

class UserManager
{
public:
    UserManager(const std::string &dbName);
    ~UserManager();

    bool createUserTable();
    bool registerUser(const User &user);
    bool authenticateUser(const std::string &email, const std::string &password);
    bool updateUserBalance(const std::string& email, double newBalance);
    std::optional<User> getUserByEmail(const std::string &email);
private:
    sqlite3 *db;
    bool openDatabase(const std::string &dbName);
    void closeDatabase();
};

#endif // USERMANAGER_H