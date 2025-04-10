#ifndef CONSOLE_NOTIFIER_H
#define CONSOLE_NOTIFIER_H

#include "BookingObserver.h"
#include <iostream>

class ConsoleNotifier : public BookingObserver {
public:
    void update(const std::string& message) override {
        std::cout << "[Notification] " << message << std::endl;
    }
};

#endif