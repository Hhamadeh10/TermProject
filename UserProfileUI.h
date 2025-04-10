#ifndef USERPROFILEUI_H
#define USERPROFILEUI_H

#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>
#include "CarManager.h"
#include "BookingManager.h"
#include "PaymentProxy.h"
#include "UserProfileUI.h"
#include "UIMediator.h"
#include "MessageManager.h"

class UserProfileUI : public QWidget {
    Q_OBJECT

public:
UserProfileUI(CarManager* carManager,
    BookingManager* bookingManager,
    PaymentProxy* paymentProxy,
    MessageManager* messageManager,
    UIMediator* mediator,
    QWidget* parent = nullptr);
private slots:
    void onBackClicked();
private:
    CarManager* carManager;
    BookingManager* bookingManager;
    PaymentInterface* paymentProxy;
    UIMediator* mediator;
    MessageManager* messageManager;

    QPushButton* backButton;
    QLabel* emailLabel;
    QLabel* balanceLabel;

    QTableWidget* listingsTable;
    QTableWidget* bookingsTable;

    void loadUserDetails();
    void loadUserListings();
    void loadUserBookings();

protected:
    void showEvent(QShowEvent* event) override;
};

#endif