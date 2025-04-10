#ifndef CARMANAGER_H
#define CARMANAGER_H

#include <string>
#include <vector>
#include <sqlite3.h>
#include "Car.h"
#include "Booking.h"
#include "BookingObserver.h"


class CarManager {
public:
    CarManager(const std::string& dbName);
    ~CarManager();

    bool createCarTable();
    bool addCar(const Car& car);
    std::vector<Car> searchAvailableCars(const std::string& location, const std::string& startDate, const std::string& endDate);
    std::vector<Car> getCarsByOwner(const std::string& email);
    std::vector<Car> getAvailableCarsExcludingUser(const std::string& userEmail, const std::string& locationFilter = "");
        
    bool isCarAvailable(int carId, const std::string& startDate, const std::string& endDate); // ✅ New
    bool bookCar(int carId, const std::string& startDate, const std::string& endDate, const std::string& renterEmail); // ✅ Updated
    void attachObserver(BookingObserver* observer);
    void notifyObservers(const std::string& message);
    bool deleteCarById(int id);
    bool updateCarById(const Car& car);
    
private:
    sqlite3* db;
    bool openDatabase(const std::string& dbName);
    void closeDatabase();

    std::vector<Car> cars;
    std::vector<BookingObserver*> observers;
    std::vector<Booking> bookings;
    

};

#endif // CARMANAGER_H