#ifndef POPUPNOTIFIER_H
#define POPUPNOTIFIER_H

#include "Observer.h"
#include <QMessageBox>
#include <QObject>

class PopupNotifier : public QObject, public Observer {
public:
    void onNotify(const std::string& message) override {
        QMessageBox::information(nullptr, "Notification", QString::fromStdString(message));
    }
};

#endif