#ifndef MAINMENUUI_H
#define MAINMENUUI_H

#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QWidget>
#include "CarManager.h"
#include <vector>
#include "UIMediator.h"

class MainMenuUI : public QWidget {
    Q_OBJECT

protected:
    void showEvent(QShowEvent* event) override;
public:
    MainMenuUI(CarManager* carManager, UIMediator* mediator, QWidget* parent = nullptr);
    void addActionButtonsToRow(int row);
    void clearCarTable();

private slots:
    void onCreateCarClicked();
    void onEditClicked(int row);
    void onDeleteClicked(int row);
    void onBackClicked();
private:
    void loadOwnerCars();  // ✅ new method

    CarManager* carManager;
    UIMediator* mediator;

    // Inputs
    QLineEdit* modelInput;
    QLineEdit* yearInput;
    QLineEdit* mileageInput;
    QLineEdit* locationInput;
    QLineEdit* availabilityInput;
    QLineEdit* priceInput;

    QPushButton* backButton;
    QPushButton* createButton;
    QTableWidget* carTable;  // ✅ to display listings

    std::vector<int> carIds;
};

#endif