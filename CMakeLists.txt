cmake_minimum_required(VERSION 3.10)
project(TermProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

# Find Qt6 Widgets
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Add static SQLite3 (optional if using system-wide SQLite)
add_library(sqlite3_c STATIC sqlite3.c)
set_target_properties(sqlite3_c PROPERTIES LINKER_LANGUAGE C)

# Source files
set(SOURCES
    main.cpp
    MainWindow.cpp
    MainWindow.h
    RegisterWindow.cpp
    RegisterWindow.h
    MainMenuUI.cpp
    MainMenuUI.h
    User.cpp
    UserSession.cpp
    Car.cpp
    UserManager.cpp
    PaymentProxy.cpp
    CarManager.cpp
    Booking.cpp
    DriveShareUIMediator.cpp
    DriveShareUIMediator.h
    EditCarDialog.cpp
    EditCarDialog.h
    BookingManager.h
    BookingManager.cpp
    RenterBrowseUI.cpp
    RenterBrowseUI.h
    DashboardUI.cpp
    UserProfileUI.h
    UserProfileUI.cpp
    PasswordRecoveryDialog.cpp
    MessageManager.cpp
    MessageUI.cpp
    Message.cpp
)

# Create the main executable
add_executable(TermProject ${SOURCES})

# Link Qt6 and SQLite
target_link_libraries(TermProject PRIVATE Qt6::Widgets sqlite3_c)