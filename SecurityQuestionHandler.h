#ifndef SECURITY_QUESTION_HANDLER_H
#define SECURITY_QUESTION_HANDLER_H

#include "PasswordRecoveryHandler.h"
#include <vector>
#include <iostream>

class SecurityQuestionHandler : public PasswordRecoveryHandler
{
private:
    int questionIndex;
    std::vector<std::string> correctAnswers;

public:
    SecurityQuestionHandler(int index, const std::vector<std::string> &answers)
        : questionIndex(index), correctAnswers(answers) {}

    bool handle(const std::vector<std::string> &userInputs)
    {
        if (userInputs[questionIndex] == correctAnswers[questionIndex])
        {
            std::cout << "Answer to question " << questionIndex + 1 << " correct.\n";
            if (nextHandler)
            {
                return nextHandler->handle(userInputs); // Pass entire input vector
            }
            return true;
        }
        else
        {
            std::cout << "Incorrect answer for question " << questionIndex + 1 << ".\n";
            return false;
        }
    }
};

#endif // SECURITY_QUESTION_HANDLER_H