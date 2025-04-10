#include "PaymentInterface.h"
#include "UserManager.h"

class RealPaymentService : public PaymentInterface {
private:
    UserManager* userManager;

public:
    RealPaymentService(UserManager* manager);

    bool processPayment(const std::string& from, const std::string& to, double amount) override;

    double getUserBalance(const std::string& email) const override;
};