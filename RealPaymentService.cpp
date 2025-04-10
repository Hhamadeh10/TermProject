#include "RealPaymentService.h"
#include <iostream>

RealPaymentService::RealPaymentService(UserManager* userManager)
    : userManager(userManager) {}

bool RealPaymentService::processPayment(const std::string& from, const std::string& to, double amount) {
    auto fromOpt = userManager->getUserByEmail(from);
    auto toOpt = userManager->getUserByEmail(to);

    if (!fromOpt.has_value() || !toOpt.has_value()) {
        std::cerr << "[RealPaymentService] User not found.\n";
        return false;
    }

    User fromUser = fromOpt.value();
    User toUser = toOpt.value();

    if (fromUser.getBalance() < amount) {
        std::cerr << "[RealPaymentService] Insufficient balance.\n";
        return false;
    }

    userManager->updateUserBalance(from, fromUser.getBalance() - amount);
    userManager->updateUserBalance(to, toUser.getBalance() + amount);

    std::cout << "[RealPaymentService] $" << amount << " transferred from " << from << " to " << to << "\n";
    return true;
}

double RealPaymentService::getUserBalance(const std::string& email) const {
    auto userOpt = userManager->getUserByEmail(email);
    return userOpt.has_value() ? userOpt->getBalance() : 0.0;
}