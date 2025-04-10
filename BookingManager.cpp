#include "BookingManager.h"
#include <iostream>
#include <ctime>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>

BookingManager::BookingManager(const std::string& dbPath) {
    openDatabase(dbPath);
}

BookingManager::~BookingManager() {
    closeDatabase();
}

bool BookingManager::openDatabase(const std::string& dbPath) {
    return sqlite3_open(dbPath.c_str(), &db) == SQLITE_OK;
}

void BookingManager::closeDatabase() {
    if (db) sqlite3_close(db);
}

bool BookingManager::createBookingTable() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS bookings (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            car_id INTEGER,
            renter_email TEXT,
            start_date TEXT,
            end_date TEXT
        );
    )";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Error creating bookings table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool BookingManager::bookCar(int carId, const std::string& renterEmail) {
    const char* sql = "INSERT INTO bookings (car_id, renter_email, start_date, end_date) VALUES (?, ?, ?, ?)";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return false;

    sqlite3_bind_int(stmt, 1, carId);
    sqlite3_bind_text(stmt, 2, renterEmail.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, "2025-04-10", -1, SQLITE_STATIC);  // example start
    sqlite3_bind_text(stmt, 4, "2025-04-11", -1, SQLITE_STATIC);  // example end

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc == SQLITE_DONE;
}

bool BookingManager::markCarAsUnavailable(int carId) {
    const char* sql = "UPDATE cars SET availability = 'Booked' WHERE id = ?";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, carId);
    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return success;
}

std::vector<Booking> BookingManager::getBookingsByRenter(const std::string& renterEmail) {
    qDebug() << "[BookingManager] getBookingsByRenter() for:" << QString::fromStdString(renterEmail);

    std::vector<Booking> bookings;

    const char* sql = "SELECT car_id, renter_email, start_date, end_date FROM bookings WHERE renter_email = ?";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        qDebug() << "[BookingManager] Failed to prepare query.";
        return bookings;
    }

    sqlite3_bind_text(stmt, 1, renterEmail.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int carId = sqlite3_column_int(stmt, 0);
        std::string email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string start = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string end = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        qDebug() << "[BookingManager] Row:" << carId << QString::fromStdString(email)
                 << QString::fromStdString(start) << QString::fromStdString(end);

        bookings.emplace_back(carId, email, start, end);
    }

    sqlite3_finalize(stmt);
    qDebug() << "[BookingManager] Returning bookings count:" << bookings.size();
    return bookings;
}