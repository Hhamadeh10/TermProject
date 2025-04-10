#ifndef RENTERBROWSEUI_H
#define RENTERBROWSEUI_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include "CarManager.h"
#include "BookingManager.h"
#include "UIMediator.h"
#include "PaymentProxy.h"
#include "NotificationCenter.h"
#include "MessageManager.h"

class RenterBrowseUI : public QWidget {
    Q_OBJECT

public:
    RenterBrowseUI(CarManager* carManager, BookingManager* bookingManager, PaymentProxy* paymentProxy, MessageManager* messageManager, UIMediator* mediator, NotificationCenter* notificationCenter, QWidget* parent = nullptr);

private slots:
    void onSearchClicked();
    void onBookClicked(int row);
    void onBackClicked();
    
private:
    CarManager* carManager;
    std::vector<Car> currentResults;
    BookingManager* bookingManager;
    UIMediator* mediator;  
    PaymentInterface* paymentProxy;
    NotificationCenter* notificationCenter;
    MessageManager* messageManager;

    QPushButton* backButton;
    QLineEdit* locationInput;
    QPushButton* searchButton;
    QTableWidget* resultTable;

    void loadResults(const std::string& locationFilter);
    void addBookButtonToRow(int row);
    void addMessageButtonToRow(int row);
    void onMessageClicked(int row);
};

#endif