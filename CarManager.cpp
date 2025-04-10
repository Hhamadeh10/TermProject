#include "CarManager.h"
#include <iostream>

CarManager::CarManager(const std::string& dbName) {
    if (!openDatabase(dbName)) {
        std::cerr << "Failed to open car database.\n";
    }
}

CarManager::~CarManager() {
    closeDatabase();
}

bool CarManager::openDatabase(const std::string& dbName) {
    return sqlite3_open(dbName.c_str(), &db) == SQLITE_OK;
}

void CarManager::closeDatabase() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool CarManager::createCarTable() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS cars (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            owner_email TEXT,
            model TEXT,
            year INTEGER,
            mileage INTEGER,
            location TEXT,
            availability TEXT,
            price_per_day REAL
        );
    )";

    

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "Failed to create cars table: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool CarManager::addCar(const Car& car) {
    const char* sql = R"(
        INSERT INTO cars (owner_email, model, year, mileage, location, availability, price_per_day)
        VALUES (?, ?, ?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare insert statement: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, car.getOwnerEmail().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, car.getModel().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, car.getYear());
    sqlite3_bind_int(stmt, 4, car.getMileage());
    sqlite3_bind_text(stmt, 5, car.getLocation().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, car.getAvailability().c_str(), -1, SQLITE_TRANSIENT); // Example: "2025-04-01 to 2025-04-07"
    sqlite3_bind_double(stmt, 7, car.getPricePerDay());

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to insert car: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    return true;
}

bool isOverlap(const std::string& start1, const std::string& end1,
    const std::string& start2, const std::string& end2) {
return !(end1 < start2 || end2 < start1);
}

bool CarManager::isCarAvailable(int carId, const std::string& startDate, const std::string& endDate) {
for (const auto& booking : bookings) {
if (booking.carId == carId && isOverlap(booking.startDate, booking.endDate, startDate, endDate)) {
 return false; // Conflict found
}
}
return true;
}

bool CarManager::bookCar(int carId, const std::string& startDate, const std::string& endDate, const std::string& renterEmail) {
if (!isCarAvailable(carId, startDate, endDate)) {
std::cout << "Booking conflict: Car is not available for the selected dates." << std::endl;
return false;
}

bookings.emplace_back(carId, renterEmail, startDate, endDate);
std::string msg = "Car ID " + std::to_string(carId) + " booked from " + startDate + " to " + endDate + " by " + renterEmail;
std::cout << msg << std::endl;
notifyObservers(msg);
return true;
}

std::vector<Car> CarManager::searchAvailableCars(const std::string& location, const std::string& startDate, const std::string& endDate) {
    std::vector<Car> result;

    for (const auto& car : cars) {
        if (car.getLocation() == location && isCarAvailable(car.getId(), startDate, endDate)) {
            result.push_back(car);
        }
    }

    return result;
}

void CarManager::attachObserver(BookingObserver* observer) {
    observers.push_back(observer);
}

void CarManager::notifyObservers(const std::string& message) {
    for (auto* observer : observers) {
        observer->update(message);
    }
}

std::vector<Car> CarManager::getCarsByOwner(const std::string& email) {
    std::vector<Car> cars;
    const char* sql = R"(
    SELECT id, model, year, mileage, location, price_per_day, availability
    FROM cars
    WHERE owner_email = ?
)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);  // ✅ car ID
            std::string model = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            int year = sqlite3_column_int(stmt, 2);
            int mileage = sqlite3_column_int(stmt, 3);
            std::string location = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            float price = static_cast<float>(sqlite3_column_double(stmt, 5));
            std::string availability = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));

            // ✅ Build the car and set its ID + owner
            Car car(model, year, mileage, location, price, availability);
            car.setId(id);                   // ✅ Track ID
            car.setOwnerEmail(email);        // ✅ Track owner

            cars.push_back(car);
        }
    }

    sqlite3_finalize(stmt);
    return cars;
}

bool CarManager::deleteCarById(int id) {
    const char* sql = "DELETE FROM cars WHERE id = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, id);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

bool CarManager::updateCarById(const Car& car) {
    const char* sql = R"(
        UPDATE cars
        SET model = ?, year = ?, mileage = ?, location = ?, price_per_day = ?, availability = ?
        WHERE id = ?
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, car.getModel().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, car.getYear());
    sqlite3_bind_int(stmt, 3, car.getMileage());
    sqlite3_bind_text(stmt, 4, car.getLocation().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 5, car.getPricePerDay());
    sqlite3_bind_text(stmt, 6, car.getAvailability().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 7, car.getId());

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

std::vector<Car> CarManager::getAvailableCarsExcludingUser(const std::string& userEmail, const std::string& locationFilter) {
    std::vector<Car> cars;
    const char* sql = R"(
        SELECT id, model, year, mileage, location, price_per_day, availability, owner_email
        FROM cars
        WHERE owner_email != ? AND availability != 'Booked' AND location LIKE ?
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, userEmail.c_str(), -1, SQLITE_STATIC);
        std::string likeStr = "%" + locationFilter + "%";
        sqlite3_bind_text(stmt, 2, likeStr.c_str(), -1, SQLITE_STATIC);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string model = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            int year = sqlite3_column_int(stmt, 2);
            int mileage = sqlite3_column_int(stmt, 3);
            std::string location = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            float price = static_cast<float>(sqlite3_column_double(stmt, 5));
            std::string availability = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
            std::string ownerEmail = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));

            Car car(model, year, mileage, location, price, availability);
            car.setId(id);
            car.setOwnerEmail(ownerEmail);
            cars.push_back(car);
        }
    }

    sqlite3_finalize(stmt);
    return cars;
}