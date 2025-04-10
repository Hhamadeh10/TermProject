#include <QApplication>
#include <QStackedWidget>
#include "UserManager.h"
#include "MainWindow.h"
#include "MainMenuUI.h"
#include "RegisterWindow.h"
#include "DriveShareUIMediator.h"
#include "RenterBrowseUI.h"
#include "DashboardUI.h"
#include "UserProfileUI.h"
#include "MessageManager.h"
#include "PopupNotifier.h"
#include "NotificationCenter.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    UserManager userManager("driveshare.db");
    userManager.createUserTable();

    CarManager carManager("driveshare.db");
    carManager.createCarTable();

    BookingManager bookingManager("driveshare.db");
    bookingManager.createBookingTable();

    MessageManager messageManager("driveshare.db");
    messageManager.createMessageTable();

    QStackedWidget stack;
    DriveShareUIMediator mediator(&stack);

    NotificationCenter* notificationCenter = new NotificationCenter();
    PopupNotifier* popup = new PopupNotifier();
    notificationCenter->addObserver(popup);

    PaymentProxy* paymentProxy = new PaymentProxy(&userManager);

    MainWindow* loginUI = new MainWindow(&userManager, &mediator);
    RegisterWindow* registerUI = new RegisterWindow(&userManager, &mediator);
    MainMenuUI* mainMenu = new MainMenuUI(&carManager, &mediator);
    RenterBrowseUI* renterUI = new RenterBrowseUI(&carManager, &bookingManager, paymentProxy, &messageManager, &mediator, notificationCenter);
    DashboardUI* dashboard = new DashboardUI(&mediator);
    UserProfileUI* profileUI = new UserProfileUI(&carManager, &bookingManager, paymentProxy, &messageManager, &mediator);

    mediator.setMainMenuUI(mainMenu);
    mediator.setUserProfileUI(profileUI);

    stack.addWidget(loginUI);     // index 0 - Login
    stack.addWidget(registerUI);  // index 1 - Register
    stack.addWidget(mainMenu);    // index 2 - Main Menu
    stack.addWidget(dashboard);   // index 3 - Main Menu
    stack.addWidget(renterUI);    // index 4 - Main Menu
    stack.addWidget(profileUI);   // index 5 - Main Menu
    
    
    stack.setWindowTitle("DriveShare");
    stack.resize(400, 500);
    stack.setCurrentIndex(0); // show login first
    stack.show();

    return app.exec();
}