#include "Message.h"

Message::Message(const std::string& sender,
                 const std::string& receiver,
                 const std::string& content,
                 const std::string& timestamp,
                 int carId)
    : sender(sender),
      receiver(receiver),
      content(content),
      timestamp(timestamp),
      carId(carId)
{}