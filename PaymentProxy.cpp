#include "PaymentProxy.h"
#include <QDebug>

PaymentProxy::PaymentProxy(UserManager* userManager) {
    realService = new RealPaymentService(userManager);
}

PaymentProxy::~PaymentProxy() {
    delete realService;
}

bool PaymentProxy::processPayment(const std::string& from, const std::string& to, double amount) {
    qDebug() << "[PaymentProxy] Processing payment...";
    return realService->processPayment(from, to, amount);
}

double PaymentProxy::getUserBalance(const std::string& email) const {
    qDebug() << "[PaymentProxy] Getting balance...";
    return realService->getUserBalance(email);
}