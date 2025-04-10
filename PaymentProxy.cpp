#include "PaymentProxy.h"
#include <iostream>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>

PaymentProxy::PaymentProxy(UserManager* userManager) : userManager(userManager) {}

bool PaymentProxy::processPayment(const std::string& fromUser, const std::string& toUser, double amount) {
    auto fromOpt = userManager->getUserByEmail(fromUser);
    auto toOpt = userManager->getUserByEmail(toUser);

    if (!fromOpt.has_value() || !toOpt.has_value()) {
        std::cerr << "User not found.\n";
        return false;
    }

    User fromUserObj = fromOpt.value();
    User toUserObj = toOpt.value();

    if (fromUserObj.getBalance() < amount) {
        std::cerr << "Insufficient balance.\n";
        return false;
    }

    fromUserObj.setBalance(fromUserObj.getBalance() - amount);
    toUserObj.setBalance(toUserObj.getBalance() + amount);

    userManager->updateUserBalance(fromUser, fromUserObj.getBalance());
    userManager->updateUserBalance(toUser, toUserObj.getBalance());

    std::cout << "[Payment] $" << amount << " transferred from " << fromUser << " to " << toUser << "\n";
    return true;
}

double PaymentProxy::getUserBalance(const std::string& email) const {
    qDebug() << "[PaymentProxy] getUserBalance called for:" << QString::fromStdString(email);

    if (userManager == nullptr) {
        qDebug() << "[PaymentProxy] userManager is null!";
        return 0.0;
    }

    auto userOpt = userManager->getUserByEmail(email);
    if (!userOpt.has_value()) {
        qDebug() << "[PaymentProxy] user not found in database.";
        return 0.0;
    }

    qDebug() << "[PaymentProxy] Found user, balance:" << userOpt->getBalance();
    return userOpt->getBalance();
}