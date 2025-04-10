#ifndef PAYMENTINTERFACE_H
#define PAYMENTINTERFACE_H

#include <string>

class PaymentInterface {
public:
    virtual ~PaymentInterface() = default;
    virtual bool processPayment(const std::string& fromUser, const std::string& toUser, double amount) = 0;
    virtual double getUserBalance(const std::string& email) const = 0;
};

#endif