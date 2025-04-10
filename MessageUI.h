#ifndef MESSAGEUI_H
#define MESSAGEUI_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include "MessageManager.h"
#include "UIMediator.h"

class MessageUI : public QWidget {
    Q_OBJECT

public:
MessageUI(const std::string& currentUserEmail,
    const std::string& otherUserEmail,
    int carId,
    MessageManager* messageManager,
    UIMediator* mediator,
    QWidget* parent = nullptr);

private slots:
    void sendMessage();
    void loadMessages();
    void onBackClicked();

private:
    std::string currentUser;
    std::string otherUser;
    int carId;
    MessageManager* messageManager;
    UIMediator* mediator;

    QListWidget* messageList;
    QLineEdit* inputField;
    QPushButton* sendButton;
    QPushButton* backButton;

    void appendMessage(const Message& msg);
};

#endif