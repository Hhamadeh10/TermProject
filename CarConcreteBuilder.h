#ifndef CARBUILDER_H
#define CARBUILDER_H

#include "CarAbstractBuilder.h"

class CarConcreteBuilder : public CarAbstractBuilder
{
private:
    Car car;

public:
    CarAbstractBuilder &setModel(const std::string &model) override {
        car.setModel(model);
        return *this;
    }

    CarAbstractBuilder &setYear(int year) override {
        car.setYear(year);
        return *this;
    }

    CarAbstractBuilder &setMileage(int mileage) override {
        car.setMileage(mileage);
        return *this;
    }

    CarAbstractBuilder &setAvailability(const std::string &availability) override {
        car.setAvailability(availability);
        return *this;
    }

    CarAbstractBuilder &setLocation(const std::string &location) override {
        car.setLocation(location);
        return *this;
    }

    CarAbstractBuilder &setPricePerDay(double price) override {
        car.setPricePerDay(price);
        return *this;
    }

    CarAbstractBuilder& setOwnerEmail(const std::string& email) override {
        car.setOwnerEmail(email);
        return *this;
    }

    Car build() override {
        return car;
    }
};

#endif