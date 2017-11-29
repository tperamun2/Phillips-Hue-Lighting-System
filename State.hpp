/**
 * @brief A header for the state class which represents a change of light state to be sent to the hue api
 * @author Gurkiran Tatla
 * @author Jake Schindler
 * @author Justine Kim
 * @author Paul Salvatore
 * @author Timal Peramune
 */

#ifndef STATE_H_
#define STATE_H_

#include "Light.hpp"

class Light;
class State {

  bool isOn;
  int brightness;
  long long hue;
  int saturation;
  int transitionTime;
  Light *light;

public:
  State(bool isOn, int brightness, long long hue, int saturation, Light *light);
  State(bool isOn, int brightness, long long hue, int transitionTime);
  ~State();

  bool isON();
  int getBrightness();
  long long getHue();
  int getSaturation();
  int getTransitionTime();
  // int getOn();

  void setON(std::string ip, std::string port);
  void setOFF(std::string ip, std::string port);
  void setHue(long long hue, std::string ip, std::string port);
  void setBrightness(int brightness, std::string ip, std::string port);
  void setTransitionTime(int transitionTime);

};

#endif
