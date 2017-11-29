//
//  User.h
//
//  Created by Jake Schindler
//
//

#ifndef Bridge_h
#define Bridge_h

#include <stdio.h>
#include <string>

class Bridge{

private:
    std::string name;
    std::string location;
    std::string ip;
    std::string port;

public:
    Bridge(std::string name, std::string location, std::string ip, std::string port);

    std::string getName();
    std::string getLocation();
    std::string getIP();
    std::string getPort();
    void setName(std::string);
    void setLocation(std::string);
    void setIP(std::string);
    void setPort(std::string);

};

#endif //Bridge_h
