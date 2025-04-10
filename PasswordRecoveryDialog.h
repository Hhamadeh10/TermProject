#ifndef PASSWORDRECOVERYDIALOG_H
#define PASSWORDRECOVERYDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include "UserManager.h"
#include "PasswordRecoveryHandler.h"

class PasswordRecoveryDialog : public QDialog {
    Q_OBJECT

public:
    PasswordRecoveryDialog(UserManager* userManager, QWidget* parent = nullptr);

private slots:
    void startRecovery();

private:
    UserManager* userManager;
    QLineEdit* emailInput;
    QPushButton* recoverBtn;

    void showSecurityQuestions(const User& user);
};

#endif