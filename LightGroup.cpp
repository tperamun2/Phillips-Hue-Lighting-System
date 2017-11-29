/**
 * @brief The lightgroup class which represents the groups of lights on the hue bridge
 * @author Gurkiran Tatla
 * @author Jake Schindler
 * @author Justine Kim
 * @author Paul Salvatore
 * @author Timal Peramune
 */

#include "LightGroup.hpp"

/**
 * LightGroup
 * @brief the constructor for the lightgroup class
 *
 * @param id a string representing the id of the light group
 * @param name a string representing the name of the light group
 * @param page a reference to the homepage
 * @return none
 */
LightGroup::LightGroup(std::string id, std::string name, HomePage *page, std::string ip, std::string port) : Wt::WPushButton(name), id(id), name(name), page(page) {
    Wt::WIOService *ioservice = new Wt::WIOService();


    ioservice->start();

    Wt::Http::Client *client = new Wt::Http::Client(*ioservice);


    client->done().connect(
      boost::bind(&LightGroup::handleGroupLights, this, _1, _2));


      client->get(ip + ":" + port + "/api/newdeveloper/groups/"+this->id);

}

/**
 * handleGroupLights
 * @brief builds the group based on an api call to determine which lights are contained within it
 *
 * @param err a boost error variable
 * @param response a wt http response variable
 * @return none
 */
void LightGroup::handleGroupLights(boost::system::error_code err, const Wt::Http::Message &response){

  Wt::Json::Object result;
  Wt::Json::parse(response.body(), result);

  for(int i = 0; i < result.size(); i++){

      try{

        Wt::Json::Array group_lights = result.get("lights");

        for(int j =0; j < group_lights.size(); j++){

            std::string light_id = group_lights[i].toString();
            Light* light = this->page->getLightWithID(light_id);
            if(light){
              this->lights.push_back(light);
            }

        }

      }catch(Wt::Json::TypeException){
          continue;
      }
  }
}

/**
 *
 * @param name
 */
void LightGroup::setName(std::string name, std::string ip, std::string port){

  Wt::WIOService *ioservice = new Wt::WIOService();
  ioservice->start();
  Wt::Http::Client *client = new Wt::Http::Client(*ioservice);

  Wt::Http::Message message = Wt::Http::Message();
  message.addHeader("Content-type", "application/json");
  message.addBodyText("{\"name\": " + name + "}");

  client->put(ip + ":" + port + "/api/newdeveloper/groups/" + this->id,
              message);

  ioservice->stop();

  this->name = name;

}

/**
 * getAllLightNames
 * @brief gets the names of every light in the group
 *
 * @param none
 * @return a string representing every light in the group
 */
std::string * LightGroup::getAllLightNames(){
  std::set<std::string> temp_set;
  std::string *str = new std::string("");

  for(auto itr = this->lights.begin();  itr != this->lights.end(); ++itr){

      temp_set.insert((*itr)->getName());

  }


  for(auto itr = temp_set.begin(); itr != temp_set.end(); ++itr){
    str->append(*itr+ ",  ");
  }

  return str;
}

/**
 * getID
 * @brief get the id of the lightgroup
 *
 * @param none
 * @return a string representing the lightgroup id
 */
std::string LightGroup::getID(){
  return this->id;
}

/**
 * getName
 * @brief get the name of the lightgroup
 *
 * @param none
 * @return a string representing the name of the lightgroup
 */
std::string LightGroup::getName(){
  return this->name;
}

/**
 * removeLight
 * @brief remove a light from the light group
 *
 * @param light the light object to take out of the light group
 * @return none
 */
void LightGroup::removeLight(Light *light){
    this->lights.erase(std::remove(this->lights.begin(), this->lights.end(), light),this->lights.end());
}

/**
 * addLight
 * @brief add a light to the light group
 *
 * @param light the light object to be removed from the group
 * @return none
 */
void LightGroup::addLight(Light *light){
  this->lights.push_back(light);
}


/**
 * setBrightness
 * @brief sets the brightness for each member of a group
 *
 * @param bri an integer representing the brightness
 * @return none
 */
void LightGroup::setBrightness(int bri, std::string ip, std::string port){
  for(int i =0; i < this->lights.size() ; i++){
    this->lights[i]->getState()->setBrightness(bri, ip, port);
  }
}

/**
 * setHue
 * @brief sets the hue for each member of a group
 *
 * @param hue a long int representing the hue
 * @return none
 */
void LightGroup::setHue(long long hue, std::string ip, std::string port){

  for(int i =0; i < this->lights.size(); i++){

    this->lights[i]->getState()->setHue(hue, ip, port);

  }
}
