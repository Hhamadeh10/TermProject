#include "User.h"

User::User(const std::string& email, const std::string& password,
    const std::vector<std::string>& questions,
    const std::vector<std::string>& answers,
    double balance)
: email(email), password(password),
securityQuestions(questions), securityAnswers(answers),
balance(balance) {}

std::string User::getEmail() const { return email; }
std::string User::getPassword() const { return password; }
const std::vector<std::string>& User::getSecurityQuestions() const {
    return securityQuestions;
}

const std::vector<std::string>& User::getSecurityAnswers() const {
    return securityAnswers;
}

double User::getBalance() const {return balance;}

void User::setBalance(double value) {balance = value;}