//
//  User.h
//
//  Created by Jake Schindler
//
//

#ifndef User_h
#define User_h

#include <stdio.h>
#include <string>
#include "Persistence.h"

class User{

private:
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string password;
public:
    User();
    void update(std::string fName, std::string lName, std::string email, std::string password);
    std::string getFirstName();
    void setFirstName(std::string name);
    std::string getLastName();
    void setLastName(std::string name);
    std::string getEmail();
    void setEmail(std::string email);
    std::string getPassword();
    void setPassword(std::string password);
};

#endif //INC_3307USER_USER_H
