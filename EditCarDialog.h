#ifndef EDITCARDIALOG_H
#define EDITCARDIALOG_H

#include <QDialog>
#include "Car.h"

class QLineEdit;
class QPushButton;

class EditCarDialog : public QDialog {
    Q_OBJECT

public:
    EditCarDialog(Car car, QWidget* parent = nullptr);
    Car getUpdatedCar() const;

private:
    Car originalCar;

    QLineEdit* modelInput;
    QLineEdit* yearInput;
    QLineEdit* mileageInput;
    QLineEdit* locationInput;
    QLineEdit* availabilityInput;
    QLineEdit* priceInput;
    QPushButton* saveButton;

private slots:
    void onSaveClicked();
};

#endif