#include "MessageUI.h"
#include <QVBoxLayout>
#include <QDateTime>
#include <QLabel>

MessageUI::MessageUI(const std::string& currentUserEmail,
    const std::string& otherUserEmail,
    int carId,
    MessageManager* messageManager,
    UIMediator* mediator,
    QWidget* parent)
: QWidget(parent),
currentUser(currentUserEmail),
otherUser(otherUserEmail),
carId(carId),
messageManager(messageManager),
mediator(mediator)
{
QVBoxLayout* layout = new QVBoxLayout(this);

QLabel* chatLabel = new QLabel(QString("Chat with %1").arg(QString::fromStdString(otherUser)), this);
layout->addWidget(chatLabel);

messageList = new QListWidget(this);
layout->addWidget(messageList);

inputField = new QLineEdit(this);
inputField->setPlaceholderText("Type a message...");
layout->addWidget(inputField);

sendButton = new QPushButton("Send", this);
layout->addWidget(sendButton);
connect(sendButton, &QPushButton::clicked, this, &MessageUI::sendMessage);

backButton = new QPushButton("Back to Dashboard", this);
layout->addWidget(backButton);
connect(backButton, &QPushButton::clicked, this, &MessageUI::onBackClicked);

loadMessages();
}
void MessageUI::sendMessage() {
    QString text = inputField->text();
    if (text.isEmpty()) return;

    std::string timestamp = QDateTime::currentDateTime().toString(Qt::ISODate).toStdString();

    Message msg(currentUser, otherUser, text.toStdString(), timestamp, carId);
    if (messageManager->sendMessage(msg)) {
        appendMessage(msg);
        inputField->clear();
    }
}

void MessageUI::appendMessage(const Message& msg) {
    QString display = QString("[%1] %2: %3")
        .arg(QString::fromStdString(msg.timestamp))
        .arg(QString::fromStdString(msg.sender))
        .arg(QString::fromStdString(msg.content));
    messageList->addItem(display);
}

void MessageUI::loadMessages() {
    messageList->clear();
    auto messages = messageManager->getMessagesBetween(currentUser, otherUser, carId);
    for (const auto& msg : messages) {
        appendMessage(msg);
    }
}

void MessageUI::onBackClicked() {
    if (mediator) {
        mediator->notify("MessageUI", "back_to_dashboard");
    }
}