#ifndef PASSWORD_RECOVERY_HANDLER_H
#define PASSWORD_RECOVERY_HANDLER_H

#include <string>
#include <vector>

class PasswordRecoveryHandler
{
protected:
    PasswordRecoveryHandler *nextHandler = nullptr;

public:
    virtual ~PasswordRecoveryHandler() = default;

    void setNextHandler(PasswordRecoveryHandler *handler)
    {
        nextHandler = handler;
    }

    virtual bool handle(const std::vector<std::string> &answer) = 0;
};

#endif