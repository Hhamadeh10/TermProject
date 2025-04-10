#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include <string>
#include "UIMediator.h"

class UIComponent {
protected:
    UIMediator* mediator = nullptr;

public:
    void setMediator(UIMediator* med) {
        mediator = med;
    }
    virtual void display() = 0;
    virtual ~UIComponent() = default;
};

#endif