#ifndef SUBJECT_H
#define SUBJECT_H

#include "Observer.h"
#include <vector>
#include <algorithm>

class Subject {
protected:
    std::vector<Observer*> observers;

public:
    virtual ~Subject() {}

    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }

    void removeObserver(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notifyAll(const std::string& message) {
        for (auto* obs : observers) {
            if (obs) {
                obs->onNotify(message);
            }
        }
    }
};

#endif