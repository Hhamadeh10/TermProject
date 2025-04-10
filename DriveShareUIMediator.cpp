#include "DriveShareUIMediator.h"
#include <iostream>
#include "UserSession.h"

void DriveShareUIMediator::notify(const std::string& sender, const std::string& event) {
    if (sender == "LoginWindow" && event == "login_success") {
        stackedWidget->setCurrentIndex(3); // DashboardUI
    } else if (sender == "LoginWindow" && event == "go_to_register") {
        stackedWidget->setCurrentIndex(1);  // ✅ index of RegisterWindow
    } else if (sender == "Dashboard" && event == "go_to_owner") {
        stackedWidget->setCurrentIndex(2); // MainMenuUI
    } else if (sender == "Dashboard" && event == "go_to_renter") {
        stackedWidget->setCurrentIndex(4); // RenterBrowseUI
    } else if (event == "logout") {
        UserSession::getInstance()->logout();
        mainMenuUI->clearCarTable();
        stackedWidget->setCurrentIndex(0); // Back to login
    } else if ((sender == "MainMenuUI" || sender == "RenterBrowseUI" || sender == "UserProfileUI" || sender == "MessageUI") && event == "back_to_dashboard") {
        stackedWidget->setCurrentIndex(3); // Dashboard index
    }else if (sender == "RegisterWindow" && event == "back_to_login") {
        stackedWidget->setCurrentIndex(0); // Login screen index
    }else if (sender == "Dashboard" && event == "go_to_profile") {
        stackedWidget->setCurrentIndex(5);  // Or whatever index your UserProfileUI is at
    }
}

void DriveShareUIMediator::setMainMenuUI(MainMenuUI* mainMenu) {
    this->mainMenuUI = mainMenu;
}
void DriveShareUIMediator::setUserProfileUI(UserProfileUI* profile) {
    this->profileUI = profile;
}