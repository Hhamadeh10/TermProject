#ifndef UI_MEDIATOR_H
#define UI_MEDIATOR_H

#include <string>

class UIMediator {
public:
    virtual void notify(const std::string& sender, const std::string& event) = 0;
    virtual ~UIMediator() = default;
};

#endif