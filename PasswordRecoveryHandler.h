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

    // Set the next handler in the chain
    void setNextHandler(PasswordRecoveryHandler *handler)
    {
        nextHandler = handler;
    }

    // Handle function to override
    virtual bool handle(const std::vector<std::string> &answer) = 0;
};

#endif // PASSWORD_RECOVERY_HANDLER_H