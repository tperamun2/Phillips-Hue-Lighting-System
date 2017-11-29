/**
 * @brief The state class which represents the state parameters for a particular light
 * @author Gurkiran Tatla
 * @author Jake Schindler
 * @author Justine Kim
 * @author Paul Salvatore
 * @author Timal Peramune
 */

#include <iostream>
#include "State.hpp"

/**
 * State
 * @brief constructor for the state variable
 *
 * @param isOn: boolean saying if light is on or off
 * @param brightness: int representing brightness
 * @param hue: long long representing hue
 * @param saturation: int representing saturation
 * @param light: Light object
 * @return N/A
 */
State::State(bool isOn, int brightness, long long hue, int saturation, Light *light) : isOn(isOn), brightness(brightness), hue(hue), saturation(saturation) {
  this->light = light;
}

State::State(bool isOn, int brightness, long long hue, int transitionTime): isOn(isOn), brightness(brightness), hue(hue), transitionTime(transitionTime) {
  this->isOn = isOn;
  this->brightness = brightness;
  this->hue = hue;
  this->transitionTime = transitionTime;
}


/**
 * ~State()
 * @brief destructor for state class
 *
 * @param N/A
 * @return N/A
 */
State::~State(){

}


/**
 * getON
 * @brief retrieves the state and returns it as a bool. it takes in no parameters.
 *
 * @param N/A
 * @return bool representing if the light is on or off
 */
bool State::isON() {
  return this->isOn;
}


/**
 * getBrightness
 * @brief retrieves the brightness and returns it as an int. it takes in no parameters.
 *
 * @param N/A
 * @return int representing brightness of the light
 */
int State::getBrightness() {
  return this->brightness;
}


/**
 * getHue
 * @brief retrieves the hue and returns it as a long long. it takes in no parameters.
 *
 * @param N/A
 * @return long long representing hue of the light
 */
long long State::getHue() {
  return this->hue;
}


/**
 * getSaturation()
 * @brief retrieves the saturation and returns it as an int. it takes in no parameters.
 *
 * @param N/A
 * @return int representing saturation of the light
 */
int State::getSaturation() {
  return this->saturation; 
}

/**
 * getTransitionTime
 * @brief retrieves the transition time for the state
 *
 * @param none
 * @return an integer representing the transition time in seconds
 */
int State::getTransitionTime(){
  return this->transitionTime;
}

/**
 * setON()
 * @brief changes the light to turn on. it takes no parameters and has no return value.
 *
 * @param N/A
 * @return N/A
 */
void State::setON(std::string ip, std::string port) {

  Wt::WIOService *ioservice = new Wt::WIOService();
  ioservice->start();
  Wt::Http::Client *client = new Wt::Http::Client(*ioservice);

  Wt::Http::Message message = Wt::Http::Message();
  message.addHeader("Content-type", "application/json");
  message.addBodyText("{\"on\":true}");

  client->put(ip + ":" + port + "/api/newdeveloper/lights/" +
                  this->light->getID() + "/state",
              message);

  ioservice->stop();

  this->isOn = true;
}

/**
 * setOFF()
 * @brief turn the light off. it takes no parameters and has no return value.
 *
 * @param N/A
 * @return N/A
 */
void State::setOFF(std::string ip, std::string port) {
  Wt::WIOService *ioservice = new Wt::WIOService();
  ioservice->start();
  Wt::Http::Client *client = new Wt::Http::Client(*ioservice);

  Wt::Http::Message message = Wt::Http::Message();
  message.addHeader("Content-type", "application/json");
  message.addBodyText("{\"on\":false}");

  client->put(ip + ":" + port + "/api/newdeveloper/lights/" +
                  this->light->getID() + "/state",
              message);
  ioservice->stop();
  this->isOn = false;
}


/**
 * setHue
 * @brief sets the hue of a light, it takes in long long hue and has no return value.
 *
 * @param hue: long long representing the hue
 * @return N/A
 */
void State::setHue(long long hue, std::string ip, std::string port) {
  Wt::WIOService *ioservice = new Wt::WIOService();
  ioservice->start();
  Wt::Http::Client *client = new Wt::Http::Client(*ioservice);
  // message->addBodyText("{\"name\": "+name+"}");
  Wt::Http::Message message = Wt::Http::Message();
  message.addHeader("Content-type", "application/json");
  // Wt::Json::Object tempObj;
  // tempObj["hue"] = hue;
  // message.addBodyText(Wt::Json::serialize(tempObj));

  std::string tempObj= "{hue: " + std::to_string(hue) + "}";
  message.addBodyText(tempObj);

  client->put(ip + ":" + port + "/api/newdeveloper/lights/" +
                  this->light->getID() + "/state",
              message);
  ioservice->stop();
  this->hue = hue;
}

/**
 * setBrightness
 * @brief changes the brightness of a light, it takes in int brightness and a parameter and has no return type
 *
 * @param brightness: int representing the brightness
 * @return N/A
 */
void State::setBrightness(int brightness, std::string ip, std::string port) {

  Wt::WIOService *ioservice = new Wt::WIOService();
  ioservice->start();
  Wt::Http::Client *client = new Wt::Http::Client(*ioservice);
  Wt::Http::Message message = Wt::Http::Message();
  message.addHeader("Content-type", "application/json");
  std::string tempObj= "{bri: " + std::to_string(brightness) + "}";
  message.addBodyText(tempObj);

  client->put(ip + ":" + port + "/api/newdeveloper/lights/" + this->light->getID() + "/state", message);

  ioservice->stop();

  this->brightness = brightness;
}


void State::setTransitionTime(int transitionTime){
  this->transitionTime = transitionTime;
}
