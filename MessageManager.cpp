#include "MessageManager.h"
#include <iostream>

MessageManager::MessageManager(const std::string& dbName) {
    openDatabase(dbName);
}

MessageManager::~MessageManager() {
    closeDatabase();
}

bool MessageManager::openDatabase(const std::string& dbName) {
    return sqlite3_open(dbName.c_str(), &db) == SQLITE_OK;
}

void MessageManager::closeDatabase() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool MessageManager::createMessageTable() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS messages (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            sender_email TEXT,
            receiver_email TEXT,
            message_text TEXT,
            timestamp TEXT,
            car_id INTEGER
        );
    )";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to create messages table: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool MessageManager::sendMessage(const Message& msg) {
    const char* sql = R"(
        INSERT INTO messages (sender_email, receiver_email, message_text, timestamp, car_id)
        VALUES (?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, msg.sender.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, msg.receiver.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, msg.content.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, msg.timestamp.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, msg.carId);

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

std::vector<Message> MessageManager::getMessagesBetween(const std::string& user1, const std::string& user2, int carId) {
    std::vector<Message> messages;

    const char* sql = R"(
        SELECT sender_email, receiver_email, message_text, timestamp, car_id
        FROM messages
        WHERE car_id = ? AND (
            (sender_email = ? AND receiver_email = ?) OR
            (sender_email = ? AND receiver_email = ?)
        )
        ORDER BY timestamp ASC;
    )";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return messages;

    sqlite3_bind_int(stmt, 1, carId);
    sqlite3_bind_text(stmt, 2, user1.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, user2.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, user2.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, user1.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string sender = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string receiver = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        int cid = sqlite3_column_int(stmt, 4);
        messages.emplace_back(sender, receiver, text, timestamp, cid);
    }

    sqlite3_finalize(stmt);
    return messages;
}