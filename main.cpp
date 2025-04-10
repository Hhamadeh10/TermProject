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

    // ✅ Initialize UserManager and ensure users table exists
    UserManager userManager("driveshare.db");
    userManager.createUserTable();

    // ✅ Initialize CarManager and ensure cars table exists
    CarManager carManager("driveshare.db");
    carManager.createCarTable();

    BookingManager bookingManager("driveshare.db");
    bookingManager.createBookingTable();

    MessageManager messageManager("driveshare.db");
    messageManager.createMessageTable();

    // ✅ Create the main stack widget and mediator
    QStackedWidget stack;
    DriveShareUIMediator mediator(&stack);

    NotificationCenter* notificationCenter = new NotificationCenter();
    PopupNotifier* popup = new PopupNotifier();
    notificationCenter->addObserver(popup);

    PaymentProxy paymentProxy(&userManager);

    // ✅ Instantiate screens and pass dependencies
    qDebug() << "Creating LoginWindow...";
    MainWindow* loginUI = new MainWindow(&userManager, &mediator);

    qDebug() << "Creating RegisterWindow...";
    RegisterWindow* registerUI = new RegisterWindow(&userManager, &mediator);

    qDebug() << "Creating MainMenuUI...";
    MainMenuUI* mainMenu = new MainMenuUI(&carManager, &mediator);

    qDebug() << "Creating RenterBrowseUI...";
    RenterBrowseUI* renterUI = new RenterBrowseUI(&carManager, &bookingManager, &paymentProxy, &messageManager, &mediator, notificationCenter);

    qDebug() << "Creating DashboardUI...";
    DashboardUI* dashboard = new DashboardUI(&mediator);

    qDebug() << "Creating UserProfileUI...";
    UserProfileUI* profileUI = new UserProfileUI(&carManager, &bookingManager, &paymentProxy, &messageManager, &mediator);

    mediator.setMainMenuUI(mainMenu);
    mediator.setUserProfileUI(profileUI);

    // ✅ Register all UI screens in stack (match indices with mediator logic)
    stack.addWidget(loginUI);     // index 0 - Login
    stack.addWidget(registerUI);  // index 1 - Register
    stack.addWidget(mainMenu);    // index 2 - Main Menu
    stack.addWidget(dashboard);
    stack.addWidget(renterUI);
    stack.addWidget(profileUI);
    
    
    // ✅ Start GUI
    stack.setWindowTitle("DriveShare");
    stack.resize(400, 500);
    stack.setCurrentIndex(0); // show login first
    stack.show();

    return app.exec();
}