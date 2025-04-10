#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

class Message {
public:
    std::string sender;
    std::string receiver;
    std::string content;
    std::string timestamp;
    int carId;

    Message(const std::string& sender,
            const std::string& receiver,
            const std::string& content,
            const std::string& timestamp,
            int carId);
};

#endif