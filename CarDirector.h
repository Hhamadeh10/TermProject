#ifndef CARDIRECTOR_H
#define CARDIRECTOR_H

#include "CarAbstractBuilder.h"
#include "UserSession.h"

class CarDirector {
    private:
        CarAbstractBuilder& builder;
    
    public:
        CarDirector(CarAbstractBuilder& builder) : builder(builder) {}
    
        Car constructCar(const std::string email, const std::string& model,
                         int year,
                         int mileage,
                         const std::string& availability,
                         const std::string& location,
                         double price) {
            
            // Get currently logged-in user's email
            std::string ownerEmail = UserSession::getInstance()->getLoggedInEmail();
    
            return builder
                .setOwnerEmail(ownerEmail)
                .setModel(model)
                .setYear(year)
                .setMileage(mileage)
                .setAvailability(availability)
                .setLocation(location)
                .setPricePerDay(price)
                .build();
        }
    };

#endif // CARDIRECTOR_H