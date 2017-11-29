/**
 * @brief A header for the lightgroup class which represents a group of lights
 * @author Gurkiran Tatla
 * @author Jake Schindler
 * @author Justine Kim
 * @author Paul Salvatore
 * @author Timal Peramune
 */

#ifndef LIGHTGROUP_H_
#define LIGHTGROUP_H_
#include "Light.hpp"
#include "HomePage.hpp"
#include <set>
class Light;
class HomePage;

class LightGroup : public Wt::WPushButton{

  std::string id;
  std::string name;

  HomePage *page;
  std::vector<Light*> lights;


  void handleGroupLights(boost::system::error_code err, const Wt::Http::Message &response);

public:
  LightGroup(std::string id, std::string name, HomePage *page, std::string ip, std::string port);
  void setName(std::string name, std::string ip, std::string port);
  void addLight(Light * light);
  void removeLight(Light* light);
  std::string getName();
  std::string getID();
  std::string *getAllLightNames();
  void setHue(long long hue, std::string ip, std::string port);
  void setBrightness(int bri, std::string ip, std::string port);

};

#endif
