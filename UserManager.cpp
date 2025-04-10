#include "UserManager.h"
#include <iostream>
#include <optional>

UserManager::UserManager(const std::string &dbName)
{
    if (!openDatabase(dbName))
    {
        std::cerr << "Failed to open database.\n";
    }
}

UserManager::~UserManager()
{
    closeDatabase();
}

bool UserManager::openDatabase(const std::string &dbName)
{
    int rc = sqlite3_open(dbName.c_str(), &db);
    return rc == SQLITE_OK;
}

void UserManager::closeDatabase()
{
    if (db)
    {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool UserManager::createUserTable()
{
    const char *sql = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            email TEXT UNIQUE NOT NULL,
            password TEXT,
            q1 TEXT, a1 TEXT,
            q2 TEXT, a2 TEXT,
            q3 TEXT, a3 TEXT,
            balance REAL DEFAULT 1000.0
        );
    )";

    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Error creating table: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool UserManager::registerUser(const User &user)
{
    const char *sql = R"(
        INSERT INTO users (email, password, q1, a1, q2, a2, q3, a3, balance)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);
    )";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare registration statement: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, user.getEmail().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user.getPassword().c_str(), -1, SQLITE_STATIC);
    for (int i = 0; i < 3; ++i) {
        sqlite3_bind_text(stmt, 3 + i * 2, user.getSecurityQuestions()[i].c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4 + i * 2, user.getSecurityAnswers()[i].c_str(), -1, SQLITE_STATIC);
    }
    sqlite3_bind_double(stmt, 9, user.getBalance());

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to register user: " << sqlite3_errmsg(db) << "\n";
    }

    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

bool UserManager::authenticateUser(const std::string &email, const std::string &password)
{
    const char *sql = "SELECT * FROM users WHERE email = ? AND password = ?;";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    bool authenticated = (rc == SQLITE_ROW);
    sqlite3_finalize(stmt);

    return authenticated;
}

std::optional<User> UserManager::getUserByEmail(const std::string &email)
{
    const char *sql = R"(
        SELECT email, password, q1, a1, q2, a2, q3, a3, balance FROM users WHERE email = ?;
    )";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
        return std::nullopt;

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        std::string email = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        std::string password = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        std::vector<std::string> questions = {
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6))};
        std::vector<std::string> answers = {
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5)),
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7))};
        double balance = sqlite3_column_double(stmt, 8);
        sqlite3_finalize(stmt);
        return User(email, password, questions, answers, balance);
    }

    sqlite3_finalize(stmt);
    return std::nullopt;
}

bool UserManager::updateUserBalance(const std::string& email, double newBalance) {
    const char* sql = R"(
        UPDATE users SET balance = ? WHERE email = ?;
    )";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_double(stmt, 1, newBalance);
    sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}