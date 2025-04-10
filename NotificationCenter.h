#ifndef NOTIFICATIONCENTER_H
#define NOTIFICATIONCENTER_H

#include "Observer.h"
#include <vector>
#include "Subject.h"

class NotificationCenter : public Subject {
private:
    std::vector<Observer*> observers;

public:
    void addObserver(Observer* obs) {
        observers.push_back(obs);
    }

    void notifyAll(const std::string& message) {
        for (auto obs : observers) {
            obs->onNotify(message);
        }
    }
};

#endif