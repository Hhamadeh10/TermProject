#include "DriveShareUIMediator.h"
#include <iostream>
#include "UserSession.h"

void DriveShareUIMediator::notify(const std::string& sender, const std::string& event) {
    if (sender == "LoginWindow" && event == "login_success") {
        stackedWidget->setCurrentIndex(3);
    } else if (sender == "LoginWindow" && event == "go_to_register") {
        stackedWidget->setCurrentIndex(1);
    } else if (sender == "Dashboard" && event == "go_to_owner") {
        stackedWidget->setCurrentIndex(2);
    } else if (sender == "Dashboard" && event == "go_to_renter") {
        stackedWidget->setCurrentIndex(4);
    } else if (event == "logout") {
        UserSession::getInstance()->logout();
        mainMenuUI->clearCarTable();
        stackedWidget->setCurrentIndex(0);
    } else if ((sender == "MainMenuUI" || sender == "RenterBrowseUI" || sender == "UserProfileUI" || sender == "MessageUI") && event == "back_to_dashboard") {
        stackedWidget->setCurrentIndex(3);
    }else if (sender == "RegisterWindow" && event == "back_to_login") {
        stackedWidget->setCurrentIndex(0);
    }else if (sender == "Dashboard" && event == "go_to_profile") {
        stackedWidget->setCurrentIndex(5);
    }
}

void DriveShareUIMediator::setMainMenuUI(MainMenuUI* mainMenu) {
    this->mainMenuUI = mainMenu;
}
void DriveShareUIMediator::setUserProfileUI(UserProfileUI* profile) {
    this->profileUI = profile;
}