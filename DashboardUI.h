#ifndef DASHBOARDUI_H
#define DASHBOARDUI_H

#include <QWidget>
#include <QPushButton>

class UIMediator;

class DashboardUI : public QWidget {
    Q_OBJECT

public:
    DashboardUI(UIMediator* mediator, QWidget* parent = nullptr);

private slots:
    void goToOwnerPage();
    void goToRenterPage();
    void logout();
    void goToProfilePage();

private:
    UIMediator* mediator;
    QPushButton* ownerBtn;
    QPushButton* renterBtn;
    QPushButton* logoutBtn;
    QPushButton* profileBtn;
};

#endif