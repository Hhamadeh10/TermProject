#include "PasswordRecoveryDialog.h"
#include <QMessageBox>
#include "SecurityQuestionHandler.h"
#include <QInputDialog>

PasswordRecoveryDialog::PasswordRecoveryDialog(UserManager* userManager, QWidget* parent)
    : QDialog(parent), userManager(userManager) {
    
    setWindowTitle("Recover Password");
    QVBoxLayout* layout = new QVBoxLayout(this);

    emailInput = new QLineEdit(this);
    emailInput->setPlaceholderText("Enter your email");

    recoverBtn = new QPushButton("Start Recovery", this);
    connect(recoverBtn, &QPushButton::clicked, this, &PasswordRecoveryDialog::startRecovery);

    layout->addWidget(emailInput);
    layout->addWidget(recoverBtn);
}

void PasswordRecoveryDialog::startRecovery() {
    std::string email = emailInput->text().toStdString();
    auto userOpt = userManager->getUserByEmail(email);

    if (!userOpt.has_value()) {
        QMessageBox::warning(this, "Error", "User not found.");
        return;
    }

    showSecurityQuestions(userOpt.value());
}

void PasswordRecoveryDialog::showSecurityQuestions(const User& user) {
    std::vector<std::string> questions = user.getSecurityQuestions();
    std::vector<std::string> answers = user.getSecurityAnswers();
    std::vector<std::string> userInputs;

    // Ask user the 3 questions using Qt dialogs
    for (int i = 0; i < 3; ++i) {
        bool ok;
        QString input = QInputDialog::getText(this, 
            QString("Security Question %1").arg(i + 1),
            QString::fromStdString(questions[i]), 
            QLineEdit::Normal, "", &ok);

        if (!ok || input.isEmpty()) {
            QMessageBox::warning(this, "Cancelled", "Password recovery cancelled.");
            return;
        }

        userInputs.push_back(input.toStdString());
    }

    // Build the chain
    auto handler1 = new SecurityQuestionHandler(0, answers);
    auto handler2 = new SecurityQuestionHandler(1, answers);
    auto handler3 = new SecurityQuestionHandler(2, answers);
    handler1->setNextHandler(handler2);
    handler2->setNextHandler(handler3);

    if (handler1->handle(userInputs)) {
        QMessageBox::information(this, "Password", QString("Your password is: %1").arg(QString::fromStdString(user.getPassword())));
    } else {
        QMessageBox::warning(this, "Failed", "One or more answers were incorrect.");
    }

    delete handler1;
    delete handler2;
    delete handler3;
}