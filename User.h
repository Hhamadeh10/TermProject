#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

class User {
    private:
        std::string email;
        std::string password;
        std::vector<std::string> securityQuestions;
        std::vector<std::string> securityAnswers;
        double balance = 1000.0;
    
    public:
    User(const std::string& email, const std::string& password,
        const std::vector<std::string>& questions,
        const std::vector<std::string>& answers,
        double balance);
    
        std::string getEmail() const;
        std::string getPassword() const;
        double getBalance() const;

        const std::vector<std::string>& getSecurityQuestions() const;
        const std::vector<std::string>& getSecurityAnswers() const;
        void setBalance(double value);
    };

#endif