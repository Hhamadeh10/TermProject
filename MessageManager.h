#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include "Message.h"
#include <vector>
#include <sqlite3.h>

class MessageManager {
private:
    sqlite3* db;
    bool openDatabase(const std::string& dbName);
    void closeDatabase();

public:
    MessageManager(const std::string& dbName);
    ~MessageManager();

    bool createMessageTable();
    bool sendMessage(const Message& message);
    std::vector<Message> getMessagesBetween(const std::string& user1, const std::string& user2, int carId);
};

#endif