#include "RegisterWindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include "User.h"

RegisterWindow::RegisterWindow(UserManager* userManager, UIMediator* mediator, QWidget* parent)
    : QWidget(parent), userManager(userManager), mediator(mediator)
{
    auto layout = new QVBoxLayout(this);

    emailInput = new QLineEdit(this);
    emailInput->setPlaceholderText("Email");

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Password");

    q1Input = new QLineEdit(this); q1Input->setPlaceholderText("Security Question 1");
    a1Input = new QLineEdit(this); a1Input->setPlaceholderText("Answer 1");
    q2Input = new QLineEdit(this); q2Input->setPlaceholderText("Security Question 2");
    a2Input = new QLineEdit(this); a2Input->setPlaceholderText("Answer 2");
    q3Input = new QLineEdit(this); q3Input->setPlaceholderText("Security Question 3");
    a3Input = new QLineEdit(this); a3Input->setPlaceholderText("Answer 3");

    registerButton = new QPushButton("Register", this);
    backButton = new QPushButton("Back to Login", this);

    connect(registerButton, &QPushButton::clicked, this, &RegisterWindow::onRegisterClicked);
    connect(backButton, &QPushButton::clicked, this, &RegisterWindow::onBackClicked);

    layout->addWidget(emailInput);
    layout->addWidget(passwordInput);
    layout->addWidget(q1Input); layout->addWidget(a1Input);
    layout->addWidget(q2Input); layout->addWidget(a2Input);
    layout->addWidget(q3Input); layout->addWidget(a3Input);
    layout->addWidget(registerButton);
    layout->addWidget(backButton);
}

void RegisterWindow::onRegisterClicked() {
    std::string email = emailInput->text().toStdString();
    std::string password = passwordInput->text().toStdString();
    std::vector<std::string> questions = {
        q1Input->text().toStdString(),
        q2Input->text().toStdString(),
        q3Input->text().toStdString()
    };
    std::vector<std::string> answers = {
        a1Input->text().toStdString(),
        a2Input->text().toStdString(),
        a3Input->text().toStdString()
    };

    User newUser(email, password, questions, answers, 1000.0);
    if (userManager->registerUser(newUser)) {
        QMessageBox::information(this, "Success", "User registered!");
        if (mediator) mediator->notify("RegisterWindow", "register_success");
    } else {
        QMessageBox::warning(this, "Error", "Registration failed. Try a different email.");
    }
}

void RegisterWindow::onBackClicked() {
    if (mediator) mediator->notify("RegisterWindow", "back_to_login");
}