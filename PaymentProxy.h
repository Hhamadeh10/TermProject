#ifndef PAYMENTPROXY_H
#define PAYMENTPROXY_H

#include "PaymentInterface.h"
#include <string>
#include "UserManager.h"
#include "RealPaymentService.h"

class PaymentProxy : public PaymentInterface {
private:
    RealPaymentService* realService;

public:
    PaymentProxy(UserManager* userManager);

    ~PaymentProxy();

    bool processPayment(const std::string& fromUser, const std::string& toUser, double amount) override;

    double getUserBalance(const std::string& email) const;
};

#endif