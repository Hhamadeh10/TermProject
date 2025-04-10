#include "DashboardUI.h"
#include <QVBoxLayout>
#include "UIMediator.h"

DashboardUI::DashboardUI(UIMediator* mediator, QWidget* parent)
    : QWidget(parent), mediator(mediator) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    ownerBtn = new QPushButton("Manage My Car Listings");
    renterBtn = new QPushButton("Browse Available Cars");
    logoutBtn = new QPushButton("Logout");
    profileBtn = new QPushButton("View Profile", this);

    layout->addWidget(ownerBtn);
    layout->addWidget(renterBtn);
    layout->addWidget(logoutBtn);
    layout->addWidget(profileBtn);

    connect(ownerBtn, &QPushButton::clicked, this, &DashboardUI::goToOwnerPage);
    connect(renterBtn, &QPushButton::clicked, this, &DashboardUI::goToRenterPage);
    connect(logoutBtn, &QPushButton::clicked, this, &DashboardUI::logout);
    connect(profileBtn, &QPushButton::clicked, this, &DashboardUI::goToProfilePage);
}

void DashboardUI::goToOwnerPage() {
    mediator->notify("Dashboard", "go_to_owner");
}

void DashboardUI::goToRenterPage() {
    mediator->notify("Dashboard", "go_to_renter");
}

void DashboardUI::logout() {
    mediator->notify("Dashboard", "logout");
}

void DashboardUI::goToProfilePage() {
    mediator->notify("Dashboard", "go_to_profile");
}