#include "EditCarDialog.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>

EditCarDialog::EditCarDialog(Car car, QWidget* parent)
    : QDialog(parent), originalCar(car) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    modelInput = new QLineEdit(QString::fromStdString(car.getModel()));
    yearInput = new QLineEdit(QString::number(car.getYear()));
    mileageInput = new QLineEdit(QString::number(car.getMileage()));
    locationInput = new QLineEdit(QString::fromStdString(car.getLocation()));
    availabilityInput = new QLineEdit(QString::fromStdString(car.getAvailability()));
    priceInput = new QLineEdit(QString::number(car.getPricePerDay()));

    QFormLayout* form = new QFormLayout();
    form->addRow("Model:", modelInput);
    form->addRow("Year:", yearInput);
    form->addRow("Mileage:", mileageInput);
    form->addRow("Location:", locationInput);
    form->addRow("Availability:", availabilityInput);
    form->addRow("Price per Day:", priceInput);
    layout->addLayout(form);

    saveButton = new QPushButton("Save");
    layout->addWidget(saveButton);
    connect(saveButton, &QPushButton::clicked, this, &EditCarDialog::onSaveClicked);
}

void EditCarDialog::onSaveClicked() {
    originalCar.setModel(modelInput->text().toStdString());
    originalCar.setYear(yearInput->text().toInt());
    originalCar.setMileage(mileageInput->text().toInt());
    originalCar.setLocation(locationInput->text().toStdString());
    originalCar.setAvailability(availabilityInput->text().toStdString());
    originalCar.setPricePerDay(priceInput->text().toDouble());

    accept();  // closes the dialog with QDialog::Accepted
}

Car EditCarDialog::getUpdatedCar() const {
    return originalCar;
}