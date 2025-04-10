#include "UserProfileUI.h"
#include "UserSession.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>
#include "MessageUI.h"

UserProfileUI::UserProfileUI(CarManager* carManager,
    BookingManager* bookingManager,
    PaymentProxy* paymentProxy,
    MessageManager* messageManager,
    UIMediator* mediator,
    QWidget* parent)
: QWidget(parent),
carManager(carManager),
bookingManager(bookingManager),
paymentProxy(paymentProxy),
messageManager(messageManager),
mediator(mediator)
{

    qDebug() << "[UserProfileUI] Constructing...";

    std::string email = UserSession::getInstance()->getLoggedInEmail();
    qDebug() << "[UserProfileUI] Logged in email:" << QString::fromStdString(email);
    
    QVBoxLayout* layout = new QVBoxLayout(this);

    emailLabel = new QLabel("Email: ");
    balanceLabel = new QLabel("Balance: ");

    listingsTable = new QTableWidget(this);
    listingsTable->setColumnCount(4);
    listingsTable->setHorizontalHeaderLabels({"Model", "Year", "Location", "Price"});

    bookingsTable = new QTableWidget(this);
    bookingsTable->setColumnCount(5);
    bookingsTable->setHorizontalHeaderLabels({"Car ID", "Start Date", "End Date", "Renter", "Message"});

    layout->addWidget(emailLabel);
    layout->addWidget(balanceLabel);
    layout->addWidget(new QLabel("My Listings:"));
    layout->addWidget(listingsTable);
    layout->addWidget(new QLabel("My Bookings:"));
    layout->addWidget(bookingsTable);

    backButton = new QPushButton("Back to Dashboard", this);
    connect(backButton, &QPushButton::clicked, this, &UserProfileUI::onBackClicked);
    layout->addWidget(backButton);
}

void UserProfileUI::loadUserDetails() {
    qDebug() << "[loadUserDetails] Entering";

    if (!paymentProxy) {
        qDebug() << "[loadUserDetails] paymentProxy is null!";
        return;
    }

    std::string email = UserSession::getInstance()->getLoggedInEmail();
    qDebug() << "[loadUserDetails] Email:" << QString::fromStdString(email);

    if (email.empty()) {
        qDebug() << "[loadUserDetails] No user logged in!";
        return;
    }

    double balance = paymentProxy->getUserBalance(email);

    if (emailLabel && balanceLabel) {
        emailLabel->setText(QString("Email: %1").arg(QString::fromStdString(email)));
        balanceLabel->setText(QString("Balance: $%1").arg(balance, 0, 'f', 2));
        qDebug() << "[loadUserDetails] Updated UI with balance:" << balance;
    } else {
        qDebug() << "[loadUserDetails] emailLabel or balanceLabel is null!";
    }
}
void UserProfileUI::loadUserListings() {
    if (!carManager || !listingsTable) {
        qDebug() << "[UserProfileUI] Null pointer in loadUserListings";
        return;
    }

    std::string email = UserSession::getInstance()->getLoggedInEmail();
    if (email.empty()) return;

    auto cars = carManager->getCarsByOwner(email);
    listingsTable->setRowCount(static_cast<int>(cars.size()));

    for (int i = 0; i < cars.size(); ++i) {
        listingsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(cars[i].getModel())));
        listingsTable->setItem(i, 1, new QTableWidgetItem(QString::number(cars[i].getYear())));
        listingsTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(cars[i].getLocation())));
        listingsTable->setItem(i, 3, new QTableWidgetItem(QString::number(cars[i].getPricePerDay())));
    }
}

void UserProfileUI::loadUserBookings() {
    std::string email = UserSession::getInstance()->getLoggedInEmail();
    auto bookings = bookingManager->getBookingsByRenter(email);

    bookingsTable->setRowCount(static_cast<int>(bookings.size()));
    bookingsTable->setColumnCount(5);
    bookingsTable->setHorizontalHeaderLabels({"Car ID", "Start Date", "End Date", "Renter", "Message"});

    for (int i = 0; i < bookings.size(); ++i) {
        const Booking& booking = bookings[i];
        bookingsTable->setItem(i, 0, new QTableWidgetItem(QString::number(booking.carId)));
        bookingsTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(booking.startDate)));
        bookingsTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(booking.endDate)));
        bookingsTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(booking.renterEmail)));

    
        QPushButton* msgBtn = new QPushButton("Message");
        connect(msgBtn, &QPushButton::clicked, this, [=]() {
            MessageUI* chat = new MessageUI(
                UserSession::getInstance()->getLoggedInEmail(),
                booking.renterEmail,
                booking.carId,
                messageManager,
                mediator
            );
            chat->show();
        });
        bookingsTable->setCellWidget(i, 4, msgBtn);
    }
}

void UserProfileUI::onBackClicked() {
    mediator->notify("UserProfileUI", "back_to_dashboard");
}

void UserProfileUI::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    std::string email = UserSession::getInstance()->getLoggedInEmail();
    if (email.empty()) {
        qDebug() << "[UserProfileUI] Skipping load — no user logged in.";
        return;
    }

    qDebug() << "[UserProfileUI] Loading profile for:" << QString::fromStdString(email);
    loadUserDetails();
    loadUserListings();
    loadUserBookings();
}