// Car.h
#ifndef CAR_H
#define CAR_H

#include <string>

class Car {
    private:
        int id;
        static int nextId;
    
        std::string ownerEmail;
        std::string model;
        int year;
        int mileage;
        std::string location;
        double pricePerDay;
        std::string availability;
    
    public:
        Car();
        Car(const std::string& model, int year, int mileage,
            const std::string& location, float pricePerDay,
            const std::string& availability);
    
        int getId() const;
        std::string getModel() const;
        int getYear() const;
        int getMileage() const;
        std::string getLocation() const;
        float getPricePerDay() const;
        std::string getAvailability() const;
        std::string getOwnerEmail() const;
    
        void setId(int id);
        void setOwnerEmail(const std::string& email);
        void setModel(std::string model);
        void setYear(int year);
        void setMileage(int mileage);
        void setLocation(std::string location);
        void setAvailability(const std::string& newAvailability);
        void setPricePerDay(float newPrice);
    };


#endif // CAR_H