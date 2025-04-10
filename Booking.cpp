#include "Booking.h"

Booking::Booking(int carId, const std::string& renterEmail, const std::string& startDate, const std::string& endDate)
    : carId(carId), renterEmail(renterEmail), startDate(startDate), endDate(endDate) {}