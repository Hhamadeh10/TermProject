#ifndef BOOKING_H
#define BOOKING_H

#include <string>

class Booking {
public:
    int carId;
    std::string renterEmail;
    std::string startDate;
    std::string endDate;

    Booking(int carId, const std::string& renterEmail, const std::string& startDate, const std::string& endDate);
};

#endif