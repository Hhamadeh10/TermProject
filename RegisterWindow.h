#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>
#include "UserManager.h"
#include "UIMediator.h"

class QLineEdit;
class QPushButton;

class RegisterWindow : public QWidget {
    Q_OBJECT

public:
    RegisterWindow(UserManager* userManager, UIMediator* mediator, QWidget* parent = nullptr);

private slots:
    void onRegisterClicked();
    void onBackClicked();

private:
    UserManager* userManager;
    UIMediator* mediator;

    QLineEdit* emailInput;
    QLineEdit* passwordInput;
    QLineEdit* q1Input;
    QLineEdit* a1Input;
    QLineEdit* q2Input;
    QLineEdit* a2Input;
    QLineEdit* q3Input;
    QLineEdit* a3Input;
    QPushButton* registerButton;
    QPushButton* backButton;
};

#endif