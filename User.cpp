//
//  UserBook.cpp
//
//  Store standard user data including firstname,lastname,email, and password in strings
//  Created by Jake Schindler
//
//

#include "User.h"
#include <string>

using namespace std;

User::User(){
    firstName ="";
    lastName ="";
    email ="";
    password ="";
}
/*
 NAME: update: User
 initializes a user with all its info
 PARAMS: strings -firstname,lastname, email, password
 RETURN: void
*/
void User::update(std::string fName, std::string lName, std::string em, std::string pass){
    firstName =fName;
    lastName =lName;
    email = em;
    password = pass;
    Persistence::storeUser(fName, lName, em, pass);
}

/*
 * getters and setters
 */
string User::getFirstName() {
    return firstName;
}
void User::setFirstName(string name) {
    this->firstName = name;
    std::string email = this->email;
    Persistence::setFirstName(email, name);
}

string User::getLastName() {
    return lastName;
}
void User::setLastName(string name) {
    this->lastName = name;
    std::string email = this->email;
    Persistence::setLastName(email, name);
}

string User::getEmail() {
    return email;
}
void User::setEmail(string newemail){
    std::string oldemail = this->email;
    std::string filename = oldemail + ".txt";
    const char * c = filename.c_str();
    this->email = newemail;
    Persistence::setEmail(oldemail, newemail);
    remove(c);
}

string User::getPassword() {
    return password;
}
void User::setPassword(string password) {
    this->password = password;
    std::string email = this->email;
    Persistence::setPassword(email, password);
}
