#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "UserManager.h"
#include "UIMediator.h"
#include "UserSession.h"

class QLineEdit;
class QPushButton;

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(UserManager* userManager, UIMediator* mediator, QWidget *parent = nullptr);

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onForgotPasswordClicked();

private:
    UserManager* userManager;
    UIMediator* mediator;

    QLineEdit* emailInput;
    QLineEdit* passwordInput;
    QPushButton* forgotPasswordBtn;
    QPushButton* loginButton;
    QPushButton* registerButton;
    
};

#endif