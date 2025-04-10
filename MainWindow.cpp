#include "MainWindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include "PasswordRecoveryDialog.h"

MainWindow::MainWindow(UserManager* userManager, UIMediator* mediator, QWidget *parent)
    : QWidget(parent), userManager(userManager), mediator(mediator)
{
    auto layout = new QVBoxLayout(this);

    emailInput = new QLineEdit(this);
    emailInput->setPlaceholderText("Email");

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Password");
    passwordInput->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login", this);
    registerButton = new QPushButton("Register", this);
    forgotPasswordBtn = new QPushButton("Forgot Password?", this);
    
    connect(forgotPasswordBtn, &QPushButton::clicked, this, &MainWindow::onForgotPasswordClicked);
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);

    layout->addWidget(emailInput);
    layout->addWidget(passwordInput);
    layout->addWidget(forgotPasswordBtn);
    layout->addWidget(loginButton);
    layout->addWidget(registerButton);
}

void MainWindow::onLoginClicked() {
    QString email = emailInput->text();
    QString password = passwordInput->text();

    if (userManager->authenticateUser(email.toStdString(), password.toStdString())) {
        UserSession::getInstance()->login(email.toStdString());
        QMessageBox::information(this, "Login", "Login successful!");
        if (mediator) mediator->notify("LoginWindow", "login_success");
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid credentials");
    }
}

void MainWindow::onRegisterClicked() {
    if (mediator) {
        mediator->notify("LoginWindow", "go_to_register");
    }
}

void MainWindow::onForgotPasswordClicked() {
    PasswordRecoveryDialog dialog(userManager, this);
    dialog.exec();
}