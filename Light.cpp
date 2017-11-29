/**
 * @brief The light class which represents a Hue light with it's associated state
 * @author Gurkiran Tatla
 * @author Jake Schindler
 * @author Justine Kim
 * @author Paul Salvatore
 * @author Timal Peramune
 */

#include "Light.hpp"
#include <Wt/WApplication>
#include <Wt/WEvent>
#include <iostream>

/**
 * Light::Light(std::string id, std::string name, HomePage *page)
 * @brief Constructor; Initialization function to create a Light
 *
 * @param id: an ID number for the Light
 * @param name: a name for the Light
 * @param HomePage *page: link to homepage
 * @return N/A
 */
Light::Light(std::string id, std::string name, HomePage *page)
    : Wt::WPushButton(name) {
  this->id = id;
  this->name = name;
  this->page = page;
}

/**
 * Light::Light(std::string id, std::string name, HomePage *page, State *state)
 * @brief Constructor; Initialization function to create a Light
 *
 * @param id: an ID number for the light
 * @param name: a name for the light
 * @param page: homepage
 * @param State: light state
 * @return N/A
 */
Light::Light(std::string id, std::string name, State *state, HomePage *page)
    : Wt::WPushButton(name) {
  this->id = id;
  this->name = name;
  this->page = page;
  this->state = state;
}

/**
 * Light::~Light()
 * @brief Deconstructor
 *
 * @param N/A
 * @return N/A
 */
Light::~Light() {
  delete this->state;
  delete this->page;
}

/**
 * Light::getState()
 * @brief Returns the State of the Light; must always be called with every light object created
 *
 * @param N/A
 * @return State: state of the light
 */
State *Light::getState() { return this->state; }

/**
 * Light::setName(std::string name)
 * @brief sets the name of the light
 *
 * @param name: name of the light
 * @return N/A
 */
void Light::setName(std::string name, std::string ip, std::string port) {

  Wt::WIOService *ioservice = new Wt::WIOService();
  ioservice->start();
  Wt::Http::Client *client = new Wt::Http::Client(*ioservice);

  Wt::Http::Message message = Wt::Http::Message();
  message.addHeader("Content-type", "application/json");
  message.addBodyText("{\"name\": " + name + "}");

  client->put(ip + ":" + port + "/api/newdeveloper/lights/" + this->id, message);

  ioservice->stop();
  this->name = name;
}

/**
 * Light::getName()
 * @brief Returns the name of the Light
 *
 * @param N/A
 * @return std::string: name of the light
 */
std::string Light::getName() { return this->name; }

/**
 * Light::getID()
 * @brief Returns the ID of the Light
 *
 * @param N/A
 * @return std::string: ID of the light
 */
std::string Light::getID() { return this->id; }

/**
 * Light::turnONorOFF()
 * @brief Turns the light on or off
 *
 * @param N/A
 * @return N/A
 */
void Light::turnONorOFF() {
  if (this->state->isON()) {
    this->state->setOFF(this->page->getCurrentBridge()->getIP(), this->page->getCurrentBridge()->getPort());
  } else
    this->state->setON(this->page->getCurrentBridge()->getIP(), this->page->getCurrentBridge()->getPort());
}

/**
 * Light::setHue(long long hue)
 * @brief Sets the hue of the Light
 *
 * @param hue: a long to represent the hue of the light
 * @return  N/A
 */
void Light::setHue(long long hue) { this->state->setHue(hue, this->page->getCurrentBridge()->getIP(), this->page->getCurrentBridge()->getPort()); }

/**
 * Light::setBrightness(int brightness)
 * @brief Sets the brightness of the Light
 *
 * @param brightness: an int to represent the brightness of the light
 * @return N/A
 */
void Light::setBrightness(int brightness) {
  this->state->setBrightness(brightness, this->page->getCurrentBridge()->getIP(), this->page->getCurrentBridge()->getPort());
}
