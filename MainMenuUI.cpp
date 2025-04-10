#include "MainMenuUI.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include "CarConcreteBuilder.h"
#include "UserSession.h"
#include "CarDirector.h"
#include <QFormLayout>
#include <QHeaderView>
#include <QTableWidget>
#include "EditCarDialog.h"
#include <QShowEvent>


MainMenuUI::MainMenuUI(CarManager* carManager, UIMediator* mediator, QWidget* parent)
    : QWidget(parent), carManager(carManager), mediator(mediator) {

    QVBoxLayout* layout = new QVBoxLayout(this);

    modelInput = new QLineEdit(this);
    yearInput = new QLineEdit(this);
    mileageInput = new QLineEdit(this);
    locationInput = new QLineEdit(this);
    availabilityInput = new QLineEdit(this);
    priceInput = new QLineEdit(this);
    createButton = new QPushButton("Create Car Listing", this);

    connect(createButton, &QPushButton::clicked, this, &MainMenuUI::onCreateCarClicked);

    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("Model:", modelInput);
    formLayout->addRow("Year:", yearInput);
    formLayout->addRow("Mileage:", mileageInput);
    formLayout->addRow("Location:", locationInput);
    formLayout->addRow("Availability:", availabilityInput);
    formLayout->addRow("Price per day:", priceInput);

    layout->addLayout(formLayout);
    layout->addWidget(createButton);
    
    
    carTable = new QTableWidget(this);
    carTable->setColumnCount(8);
    carTable->setHorizontalHeaderLabels({"Model", "Year", "Mileage", "Location", "Availability", "Price", "Edit", "Delete"});
    carTable->horizontalHeader()->setStretchLastSection(true);
    layout->addWidget(carTable);

    backButton = new QPushButton("Back to Dashboard", this);
    connect(backButton, &QPushButton::clicked, this, &MainMenuUI::onBackClicked);
    layout->addWidget(backButton);

    loadOwnerCars();
}

void MainMenuUI::onCreateCarClicked() {
    QString model = modelInput->text();
    int year = yearInput->text().toInt();
    int mileage = mileageInput->text().toInt();
    QString location = locationInput->text();
    QString availability = availabilityInput->text();
    double price = priceInput->text().toDouble();

    std::string ownerEmail = UserSession::getInstance()->getLoggedInEmail();

    CarConcreteBuilder builder;
    CarDirector director(builder);

    Car car = director.constructCar(
        ownerEmail,
        model.toStdString(),
        year,
        mileage,
        availability.toStdString(),
        location.toStdString(),
        price
    );

    car.setOwnerEmail(ownerEmail);

    if (carManager->addCar(car)) {
        QMessageBox::information(this, "Success", "Car listing created!");
        loadOwnerCars();
    } else {
        QMessageBox::warning(this, "Error", "Failed to save car to database.");
    }
}

void MainMenuUI::loadOwnerCars() {
    std::string email = UserSession::getInstance()->getLoggedInEmail();
    std::vector<Car> cars = carManager->getCarsByOwner(email);

    carTable->setRowCount(static_cast<int>(cars.size()));
    carIds.clear();  // ✅ clear previous IDs

    for (int i = 0; i < cars.size(); ++i) {
        const Car& car = cars[i];
        carIds.push_back(car.getId());  // ✅ store car ID for that row

        carTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(car.getModel())));
        carTable->setItem(i, 1, new QTableWidgetItem(QString::number(car.getYear())));
        carTable->setItem(i, 2, new QTableWidgetItem(QString::number(car.getMileage())));
        carTable->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(car.getLocation())));
        carTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(car.getAvailability())));
        carTable->setItem(i, 5, new QTableWidgetItem(QString::number(car.getPricePerDay())));

        addActionButtonsToRow(i);  // ✅ add edit/delete buttons
    }
}

void MainMenuUI::addActionButtonsToRow(int row) {
    // Edit button
    QPushButton* editBtn = new QPushButton("Edit");
    connect(editBtn, &QPushButton::clicked, this, [=]() { onEditClicked(row); });
    carTable->setCellWidget(row, 6, editBtn);

    // Delete button
    QPushButton* deleteBtn = new QPushButton("Delete");
    connect(deleteBtn, &QPushButton::clicked, this, [=]() { onDeleteClicked(row); });
    carTable->setCellWidget(row, 7, deleteBtn);
}

void MainMenuUI::onDeleteClicked(int row) {
    int carId = carIds[row];  // ✅ get ID for this row

    if (QMessageBox::question(this, "Delete", "Are you sure you want to delete this listing?") == QMessageBox::Yes) {
        if (carManager->deleteCarById(carId)) {
            QMessageBox::information(this, "Deleted", "Car deleted.");
            loadOwnerCars(); // refresh
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete car.");
        }
    }
}

void MainMenuUI::onEditClicked(int row) {
    int carId = carIds[row];
    std::string ownerEmail = UserSession::getInstance()->getLoggedInEmail();
    
    // Rebuild the Car object from the table data
    Car car(
        carTable->item(row, 0)->text().toStdString(),  // model
        carTable->item(row, 1)->text().toInt(),        // year
        carTable->item(row, 2)->text().toInt(),        // mileage
        carTable->item(row, 3)->text().toStdString(),  // location
        carTable->item(row, 5)->text().toDouble(),     // price
        carTable->item(row, 4)->text().toStdString()   // availability
    );
    car.setId(carId);
    car.setOwnerEmail(ownerEmail);
    if (car.getOwnerEmail() != UserSession::getInstance()->getLoggedInEmail()) {
            QMessageBox::warning(this, "Unauthorized", "You can only edit your own listings.");
            return;
        }
    EditCarDialog dialog(car, this);
    if (dialog.exec() == QDialog::Accepted) {
        Car updatedCar = dialog.getUpdatedCar();
        if (carManager->updateCarById(updatedCar)) {
            QMessageBox::information(this, "Updated", "Car listing updated.");
            loadOwnerCars();  // ✅ Refresh the table
        } else {
            QMessageBox::warning(this, "Error", "Failed to update car.");
        }
    }
}

void MainMenuUI::onBackClicked() {
    mediator->notify("MainMenuUI", "back_to_dashboard");
}

void MainMenuUI::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);  // Call base class method

    // ✅ Reload listings for current user when screen is shown
    loadOwnerCars();
}

void MainMenuUI::clearCarTable() {
    carTable->setRowCount(0);
    carIds.clear();
}