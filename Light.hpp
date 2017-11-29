/**
 * @brief The header for the light class
 * @author Gurkiran Tatla
 * @author Jake Schindler
 * @author Justine Kim
 * @author Paul Salvatore
 * @author Timal Peramune
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include <string>
#include <Wt/WPushButton>
#include <Wt/Json/Value>
#include <Wt/Json/Array>
#include <Wt/Json/Object>
#include <Wt/Json/Serializer>
#include <Wt/Json/Parser>
#include <Wt/Http/Message>
#include <Wt/Http/Client>
#include <Wt/Http/Request>
#include <Wt/WLineEdit>
#include "State.hpp"
#include "HomePage.hpp"

class State;
class HomePage;
class Light : public Wt::WPushButton {

  std::string id;
  std::string name;
  State *state;
  HomePage *page;

public:
  Light(std::string id, std::string name, HomePage *page);
  Light(std::string id,std::string name, State *state, HomePage *page);
  Light(std::string id,std::string name, HomePage *page, State *state);
  ~Light();
  State* getState();
  void setName(std::string name, std::string ip, std::string port);
  std::string getName();
  std::string getID();
  void turnONorOFF();
  void setHue(long long hue);
  void setBrightness(int brightness);
};

#endif

