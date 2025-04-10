#include "RenterBrowseUI.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QMessageBox>
#include "UserSession.h"
#include "MessageUI.h"

RenterBrowseUI::RenterBrowseUI(CarManager* carManager, BookingManager* bookingManager, PaymentProxy* paymentProxy, MessageManager* messageManager, UIMediator* mediator, NotificationCenter* notificationCenter, QWidget* parent)
    : QWidget(parent), carManager(carManager), bookingManager(bookingManager), mediator(mediator), notificationCenter(notificationCenter) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    this->paymentProxy = paymentProxy;

    locationInput = new QLineEdit(this);
    locationInput->setPlaceholderText("Enter location to search");

    searchButton = new QPushButton("Search", this);
    connect(searchButton, &QPushButton::clicked, this, &RenterBrowseUI::onSearchClicked);

    resultTable = new QTableWidget(this);
    resultTable->setColumnCount(8);
    resultTable->setHorizontalHeaderLabels({"Model", "Year", "Mileage", "Location", "Availability", "Price", "Book", "Message"});
    resultTable->horizontalHeader()->setStretchLastSection(true);

    layout->addWidget(locationInput);
    layout->addWidget(searchButton);
    layout->addWidget(resultTable);

    backButton = new QPushButton("Back to Dashboard", this);
    connect(backButton, &QPushButton::clicked, this, &RenterBrowseUI::onBackClicked);
    layout->addWidget(backButton);
}

void RenterBrowseUI::onSearchClicked() {
    std::string location = locationInput->text().toStdString();
    loadResults(location);
}

void RenterBrowseUI::loadResults(const std::string& locationFilter) {
    resultTable->setRowCount(0);
    currentResults = carManager->getAvailableCarsExcludingUser(UserSession::getInstance()->getLoggedInEmail(), locationFilter);

    resultTable->setRowCount(currentResults.size());

    for (int i = 0; i < currentResults.size(); ++i) {
        const Car& car = currentResults[i];

        resultTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(car.getModel())));
        resultTable->setItem(i, 1, new QTableWidgetItem(QString::number(car.getYear())));
        resultTable->setItem(i, 2, new QTableWidgetItem(QString::number(car.getMileage())));
        resultTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(car.getLocation())));
        resultTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(car.getAvailability())));
        resultTable->setItem(i, 5, new QTableWidgetItem(QString::number(car.getPricePerDay())));

        addBookButtonToRow(i);
        addMessageButtonToRow(i);
    }
}

void RenterBrowseUI::addBookButtonToRow(int row) {
    QPushButton* bookBtn = new QPushButton("Book");
    connect(bookBtn, &QPushButton::clicked, this, [=]() { onBookClicked(row); });
    resultTable->setCellWidget(row, 6, bookBtn);
}

void RenterBrowseUI::onBookClicked(int row) {
    const Car& car = currentResults[row];
    std::string renterEmail = UserSession::getInstance()->getLoggedInEmail();
    std::string ownerEmail = car.getOwnerEmail();
    double price = car.getPricePerDay();

    if (paymentProxy->processPayment(renterEmail, ownerEmail, price)) {
        if (bookingManager->bookCar(car.getId(), renterEmail)) {
            bookingManager->markCarAsUnavailable(car.getId());
            notificationCenter->notifyAll("Booking confirmed and payment made!");
            QMessageBox::information(this, "Booking Confirmed", 
                QString("Payment of $%1 made. Car booked successfully!")
                .arg(price));
            loadResults(locationInput->text().toStdString());
        } else {
            QMessageBox::warning(this, "Booking Failed", "Could not save booking.");
        }
    } else {
        QMessageBox::warning(this, "Payment Failed", "Insufficient funds.");
    }
}

void RenterBrowseUI::onBackClicked() {
    mediator->notify("RenterBrowseUI", "back_to_dashboard");
}

void RenterBrowseUI::addMessageButtonToRow(int row) {
    QPushButton* messageBtn = new QPushButton("Message");
    connect(messageBtn, &QPushButton::clicked, this, [=]() {
        onMessageClicked(row);
    });
    resultTable->setCellWidget(row, 7, messageBtn);
}

void RenterBrowseUI::onMessageClicked(int row) {
    const Car& car = currentResults[row];
    MessageUI* chat = new MessageUI(
        UserSession::getInstance()->getLoggedInEmail(),
        car.getOwnerEmail(),
        car.getId(),
        messageManager,
        mediator,     
        this
    );

    notificationCenter->notifyAll("Message sent to owner!");
    chat->show();
}