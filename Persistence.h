/**
 * @brief A header for the persistence class which contains functions for session-to-session persistence fo data
 * @author Gurkiran Tatla
 * @author Jake Schindler
 * @author Justine Kim
 * @author Paul Salvatore
 * @author Timal Peramune
 */

#ifndef PERSISTENCE_H_
#define PERSISTENCE_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class Persistence {
public:
  static std::string encryptDecrypt(std::string toEncrypt);
  static bool storeUser(std::string fname, std::string lname, std::string email, std::string password);
  static std::string getFirstName(std::string email);
  static std::string getLastName(std::string email);
  static std::string getPassword(std::string email);
  static void setFirstName(std::string email, std::string newFirstName);
  static void setLastName(std::string email, std::string newLastName);
  static void setEmail(std::string email, std::string newEmail);
  static void setPassword(std::string email, std::string newPassword);
  static bool deleteUser(std::string email); //delete all their groups as well

  // static int incrementFilenum(int filenum);
  // static bool makeGroup(std::string email, int groupID, std::string groupname);
  // static bool makeGroup(std::string email, std::string groupname);
  // static bool addLightToGroup(std::string email, int groupID, int lightID);
  // static bool deleteLightFromGroup(std::string email, int groupID, int lightID);
  // static bool deleteGroup(std::string email, int groupID);

  static int incrementLightNum(int lightNum);
  static bool makeLight(std::string bridgeID, int lightID);
  static bool makeLight(std::string bridgeID);
  static bool deleteLight(std::string bridgeID);
  static bool storeData(std::string bridgeID, int lightID, int day, int hour, int state);

private:
};

#endif //PERSISTENCE_H
