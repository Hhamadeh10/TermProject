#ifndef BOOKINGMANAGER_H
#define BOOKINGMANAGER_H

#include <string>
#include <vector>
#include "Booking.h"
#include "sqlite3.h"

class BookingManager {
public:
    BookingManager(const std::string& dbPath);
    ~BookingManager();

    bool createBookingTable();
    bool bookCar(int carId, const std::string& renterEmail);
    bool markCarAsUnavailable(int carId);
    std::vector<Booking> getBookingsByRenter(const std::string& renterEmail);

private:
    sqlite3* db;
    bool openDatabase(const std::string& dbPath);
    void closeDatabase();
};

#endif