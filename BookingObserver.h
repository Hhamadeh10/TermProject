#ifndef BOOKINGOBSERVER_H
#define BOOKINGOBSERVER_H

#include <string>

class BookingObserver {
public:
    virtual void update(const std::string& message) = 0;
    virtual ~BookingObserver() = default;
};

#endif