#ifndef DRIVESHARE_UIMEDIATOR_H
#define DRIVESHARE_UIMEDIATOR_H

#include "UIMediator.h"
#include <QStackedWidget>
#include <QString>
#include "MainMenuUI.h"
#include "UserProfileUI.h"

class DriveShareUIMediator : public QObject, public UIMediator {
    Q_OBJECT

private:
    QStackedWidget* stackedWidget;
    MainMenuUI* mainMenuUI = nullptr;
    UserProfileUI* profileUI = nullptr;
public:
    DriveShareUIMediator(QStackedWidget* stack) : stackedWidget(stack) {}

    void notify(const std::string& sender, const std::string& event) override;
    void setMainMenuUI(MainMenuUI* mainMenu);
    void setUserProfileUI(UserProfileUI* profile);
};

#endif
