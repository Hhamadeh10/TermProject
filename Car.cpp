#include "Car.h"
#include <iostream>

int Car::nextId = 1;

Car::Car() : id(nextId++), year(0), mileage(0), pricePerDay(0.0f) {}
Car::Car(const std::string& model, int year, int mileage,
         const std::string& location, float pricePerDay,
         const std::string& availability)
    : model(model), year(year), mileage(mileage),
      location(location), pricePerDay(pricePerDay),
      availability(availability) {
    id = nextId++;
}

int Car::getId() const { return id; }
std::string Car::getModel() const { return model; }
int Car::getYear() const { return year; }
int Car::getMileage() const { return mileage; }
std::string Car::getLocation() const { return location; }
float Car::getPricePerDay() const { return pricePerDay; }
std::string Car::getAvailability() const { return availability; }
std::string Car::getOwnerEmail() const { return ownerEmail; }

void Car::setId(int id) {this->id = id;}
void Car::setOwnerEmail(const std::string& email) { ownerEmail = email; }
void Car::setModel(std::string model) { this->model = model; }
void Car::setYear(int year) { this->year = year; }
void Car::setMileage(int mileage) { this->mileage = mileage; }
void Car::setLocation(std::string location) { this->location = location; }
void Car::setAvailability(const std::string& newAvailability) { availability = newAvailability; }
void Car::setPricePerDay(float newPrice) { pricePerDay = newPrice; }