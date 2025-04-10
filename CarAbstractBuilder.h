#ifndef CARABSTRACTBUILDER_H
#define CARABSTRACTBUILDER_H

#include <string>
#include "Car.h"

class CarAbstractBuilder
{
public:
    virtual ~CarAbstractBuilder() = default;

    virtual CarAbstractBuilder &setModel(const std::string &model) = 0;
    virtual CarAbstractBuilder &setYear(int year) = 0;
    virtual CarAbstractBuilder &setMileage(int mileage) = 0;
    virtual CarAbstractBuilder &setAvailability(const std::string &availability) = 0;
    virtual CarAbstractBuilder &setLocation(const std::string &location) = 0;
    virtual CarAbstractBuilder &setPricePerDay(double price) = 0;
    virtual CarAbstractBuilder &setOwnerEmail(const std::string& email) = 0;

    virtual Car build() = 0;
};
#endif