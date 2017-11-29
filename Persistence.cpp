/**
 * @brief The persistence class which contains functions for session-to-session persistence fo data
 * @author Gurkiran Tatla
 * @author Jake Schindler
 * @author Justine Kim
 * @author Paul Salvatore
 * @author Timal Peramune
 */

#include "Persistence.h"


std::string Persistence::encryptDecrypt(std::string toEncrypt) {
  char key = 'K';
  char key1 = 'L';
  std::string output = toEncrypt;

  for (int i = 0; i < toEncrypt.size(); i++){
    output[i] = toEncrypt[i] ^ key;
    char c =output[i];
    if (isspace(c)){
      output[i] = toEncrypt[i] ^ key1;
    }
  }


  return output;
}


bool Persistence::storeUser(std::string fname, std::string lname, std::string email, std::string password){
  std::string userFile = email + ".txt";
  std::ifstream ifile(userFile);
  password=encryptDecrypt(password);

  if (ifile) {    //this file already exists
    std::cout << "This account already exists" << std::endl;
    return false;

  }
  else{           //doesnt exist, make a new one
    std::ofstream file(userFile);
    file << fname + '\n';
    file << lname + '\n';
    file << email + '\n';
    file << password + '\n';

    userFile = email + "Bridges.txt";
    std::ofstream file2(userFile);

    return true;
  }
}


std::string Persistence::getFirstName(std::string email){
  std::string userFile = email + ".txt";
  std::ifstream ifile(userFile);


  if (ifile) {    //this file already exists
    char output[100];
    if (ifile.is_open()) {
      int counter = 0;
      while (!ifile.eof()) {
        counter++;
        ifile >> output;
        if (counter==1){ //first line is name
          return output;
        }

      }
    }
  }
  else {           //doesnt exist, make a new one
    return "";
  }
  return "";
}


std::string Persistence::getLastName(std::string email){
  std::string userFile = email + ".txt";
  std::ifstream ifile(userFile);


  if (ifile) {    //this file already exists
    char output[100];
    if (ifile.is_open()) {
      int counter = 0;
      while (!ifile.eof()) {
        counter++;
        ifile >> output;
        if (counter==2){ //last name is 2nd line
          return output;
        }

      }
    }
  }
  else{           //doesnt exist, make a new one
    return "";
  }
  return "";
}


std::string Persistence::getPassword(std::string email){
  std::string userFile = email + ".txt";
  std::ifstream ifile(userFile);


  if (ifile) {    //this file already exists
    char output[100];
    if (ifile.is_open()) {
      int counter = 0;
      while (!ifile.eof()) {
        counter++;
        ifile >> output;
        if (counter==4){ //password is 4th line
          std::string password = encryptDecrypt(output);
          return password;
        }

      }
    }
  }
  else{           //doesnt exist, make a new one
    return "";
  }
  return "";
}


void Persistence::setFirstName(std::string email, std::string newFirstName){
  std::string userFile = email + ".txt";
  std::ifstream ifile(userFile);
  std::string lname;
  std::string password;
  //collect text file info
  char output[100];
  if (ifile.is_open()) {
    int counter = 0;
    while (!ifile.eof()) {
      counter++;
      ifile >> output;
      if (counter==2){ //lname is 2nd line
        lname=output;
      }
      if (counter==4){ //password is 4th line
        password=output;
      }
    }
  }
  std::ofstream file(userFile);
  file << newFirstName + '\n';
  file << lname + '\n';
  file << email + '\n';
  file << password + '\n';
}


void Persistence::setLastName(std::string email, std::string newLastName){
  std::string userFile = email + ".txt";
  std::ifstream ifile(userFile);
  std::string fname;
  std::string password;
  //collect text file info
  char output[100];
  if (ifile.is_open()) {
    int counter = 0;
    while (!ifile.eof()) {
      counter++;
      ifile >> output;
      if (counter==1){ //fname is 1st line
        fname=output;
      }
      if (counter==4){ //password is 4th line
        password=output;
      }
    }
  }
  std::ofstream file(userFile);
  file << fname + '\n';
  file << newLastName + '\n';
  file << email + '\n';
  file << password + '\n';

}


void Persistence::setEmail(std::string email, std::string newEmail){
  std::string userFile = email + ".txt";
  std::ifstream ifile(userFile);
  std::string fname;
  std::string lname;
  std::string password;
  //collect text file info
  char output[100];
  if (ifile.is_open()) {
    int counter = 0;
    while (!ifile.eof()) {
      counter++;
      ifile >> output;
      if (counter==1){ //fname is 1st line
        fname=output;
      }
      if (counter==2){ //fname is 1st line
        lname=output;
      }
      if (counter==4){ //password is 4th line
        password=output;
      }
    }
  }
  userFile = newEmail + ".txt";
  std::ofstream file(userFile);
  file << fname + '\n';
  file << lname + '\n';
  file << newEmail + '\n';
  file << password + '\n';

  userFile = newEmail + "Bridges.txt";
  std::ofstream file2(userFile);
  file2 << fname + '\n';
  file2 << lname + '\n';
  file2 << newEmail + '\n';
  file2 << password + '\n';
}


void Persistence::setPassword(std::string email, std::string newPassword){
  std::string userFile = email + ".txt";
  std::ifstream ifile(userFile);
  std::string fname;
  std::string lname;
  //collect text file info
  char output[100];
  if (ifile.is_open()) {
    int counter = 0;
    while (!ifile.eof()) {
      counter++;
      ifile >> output;
      if (counter==1){ //fname is 1st line
        fname=output;
      }
      if (counter==2){ //password is 4th line
        lname=output;
      }
    }
  }
  std::ofstream file(userFile);
  file << fname + '\n';
  file << lname + '\n';
  file << email + '\n';
  file << encryptDecrypt(newPassword) + '\n';
  //need to hash password still
}


bool Persistence::deleteUser(std::string email){
  std::string filename = email + ".txt";
  if(remove((filename).c_str())){
    return true;
  }else{
    return false;
  };

}
//delete all their groups as well


///*
//* incrementFilenum(int filenum)
//* Static method that increments the given integer by 1
//* @param: int filenum: integer to increment
//* @return: incremented integer
//*/
//int Persistence::incrementFilenum(int filenum) {
//    filenum += 1;
//    return filenum;
//}

///*
//* makeGroup(std::string userID, int groupID, std::string groupname)
//* Static method that creates a new light group for a given user
//* @param: std::string email: user's unique email
//* @param: int groupID: group number for the user
//* @param: std::string groupname: user can give the group a descriptor
//* @return: true if group is successfully made, false otherwise
//*/
//bool Persistence::makeGroup(std::string email, int groupID, std::string groupname) {
//    std::string filename = email + "group" + std::to_string(groupID) + ".txt";
//    std::ifstream ifile(filename);
//    if (ifile) {    //this file already exists
//        std::cout << "This group already exists" << std::endl;
//        return false;
//
//    }
//    else{           //doesnt exist, make a new one
//        std::ofstream file(filename);
//        file << groupname + '\n';
//        return true;
//    }
//}
//
///*
//* makeGroup(std::string userID, std::string groupname)
//* Static method that creates the first new light group for a given user
//* @param: std::string email: user's unique email
//* @param: std::string groupname: user can give the group a descriptor
//* @return: true if group is successfully made, false otherwise
//*/
////bool Persistence::makeGroup(std::string email, std::string groupname) {
////    int filenum=0;
////    std::string filename = email + "group" + std::to_string(filenum) + ".txt";
////    std::ifstream infile(filename.c_str());
////    std::ofstream groupfile;
////    if(infile) {
////        infile.close();
////        int newfilenum = incrementFilenum(filenum);
////        makeGroup(email, newfilenum, groupname);
////    }
////    else {
////        groupfile.open(filename.c_str());
////        groupfile << groupname;
////        groupfile.close();
////    }
////    return 0;
////}
//
///*
//* addLightToGroup(std::string userID, int groupID, int lightID)
//* Static method that adds a new lightID to a given group owned by a given user
//* @param: std::string email: user's unique email
//* @param: int groupID: group number for the user
//* @param: int lightID: ID number for the light
//* @return: true if light is successfully added, false otherwise
//*/
//bool Persistence::addLightToGroup(std::string email, int groupID, int lightID) {
//    std::string filename = email + "group" + std::to_string(groupID) + ".txt";
//    std::string newLight = std::to_string(lightID);
//    std::ifstream infile(filename.c_str());
//    std::ofstream groupfile;
//    groupfile.open(filename.c_str(), std::ios_base::app);
//    if(!infile) {
//        std::cout << "That group doesn't exist" << std::endl;
//        std::remove(filename.c_str());
//        return true;
//    }
//    std::string line;
//    while(std::getline(infile, line)) {
//        if(line == std::to_string(lightID)) {
//            std::cout << "That light already exists" << std::endl;
//            return true;
//        }
//    }
//    groupfile << newLight.c_str();
//    groupfile << "\n";
//    groupfile.close();
//    return false;
//}
//
///*
//* deleteLightFromGroup(std::string userID, int groupID, int lightID)
//* Static method that deletes a lightID from a given group owned by a given user
//* @param: std::string email: user's unique email
//* @param: int groupID: group number for the user
//* @param: int lightID: ID number for the light
//* @return: true if light is successfully deleted, false otherwise
//*/
//bool Persistence::deleteLightFromGroup(std::string email, int groupID, int lightID) {
//    std::string filename = email + "group" + std::to_string(groupID) + ".txt";
//    std::string deletedLight = std::to_string(lightID);
//    std::ifstream groupfile(filename.c_str());
//    if(!groupfile) {
//        std::cout << "That group doesn't exist" << std::endl;
//        std::remove(filename.c_str());
//        return 1;
//    }
//    std::string line;
//    std::ofstream tempfile;
//    tempfile.open("tempfile.txt");
//    while(std::getline(groupfile, line)) {
//        if(line != deletedLight && !line.empty()) {
//            tempfile << "\n";
//            tempfile << line;
//        }
//    }
//    tempfile.close();
//    groupfile.close();
//    std::remove(filename.c_str());
//    std::rename("tempfile.txt", filename.c_str());
//    return 0;
//}
//
///*
//* deleteGroup(std::string userID, int groupID)
//* Static method that deletes a specified group owned by a given user
//* @param: std::string email: user's unique email
//* @param: int groupID: group number for the user
//* @return: true if group is successfully deleted, false otherwise
//*/
//bool Persistence::deleteGroup(std::string email, int groupID) {
//    std::string filename = email + "group" + std::to_string(groupID) + ".txt";
//    if(std::remove(filename.c_str()) != 0) {
//        perror("That group doesn't exist");
//        return 1;
//    }
//    else {
//        puts("Group successfully deleted");
//    }
//    return 0;
//}

static int incrementLightNum(int lightNum) {
  lightNum += 1;
  return lightNum;
}

static bool makeLight(std::string bridgeID, int lightID) {
  std::string filename = bridgeID + "light" + std::to_string(lightID) + ".txt";
  std::ifstream infile(filename.c_str());
  if(infile) {
    infile.close();
    int newLightID = incrementLightNum(lightID);
    return makeLight(bridgeID, newLightID);
  }
  else {
    std::ofstream lightFile;
    lightFile.open(filename.c_str());
    lightFile.close();
    std::cout << "Light successfully created" << std::endl;
  }
  return 0;
}

static bool makeLight(std::string bridgeID) {
  int lightID = 0;
  std::string filename = bridgeID + "light" + std::to_string(lightID) + ".txt";
  std::ifstream infile(filename.c_str());
  if(infile) {
    infile.close();
    int newLightID = incrementLightNum(lightID);
    return makeLight(bridgeID, newLightID);
  }
  else {
    std::ofstream lightFile;
    lightFile.open(filename.c_str());
    lightFile.close();
    std::cout << "Light successfully created" << std::endl;
  }
  return 0;
}

static bool deleteLight(std::string bridgeID, int lightID) {
  std::string filename = bridgeID + "light" + std::to_string(lightID) + ".txt";
  if(std::remove(filename.c_str()) != 0) {
    perror("That light doesn't exist");
    return 1;
  }
  else {
    puts("Light successfully deleted");
  }
  return 0;
}

/*
* Day: 1-7 (Sunday-Saturday)
* Hour: 0-23 (12am-11pm)
* State: 0, 1 (off, on)
*/
static bool storeData(std::string bridgeID, int lightID, int day, int hour, int state) {
  if(day < 1 || day > 7) {
    std::cout << "Please enter a valid day" << std::endl;
    return 1;
  }
  else if(hour < 0 || hour > 23) {
    std::cout << "Please enter a valid time" << std::endl;
    return 1;
  }
  else if(state < 0 || state > 1) {
    std::cout << "Please enter a valid state" << std::endl;
    return 1;
  }
  else {
    std::string filename = bridgeID + "light" + std::to_string(lightID) + ".txt";
    std::ifstream infile(filename.c_str());
    if(!infile) {
      std::cout << "That light doesn't exist" << std::endl;
      std::remove(filename.c_str());
      return 1;
    }
    std::ofstream groupfile;
    groupfile.open(filename.c_str(), std::ios_base::app);
    groupfile << std::to_string(day) << "\t" << std::to_string(hour) << "\t" << std::to_string(state) << std::endl;
    return 0;
  }
}
