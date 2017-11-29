/**
 * @brief The scheduleitem class which represents a scheduled event to send to the hue api
 * @author Gurkiran Tatla
 * @author Jake Schindler
 * @author Justine Kim
 * @author Paul Salvatore
 * @author Timal Peramune
 */

#include "ScheduleItem.hpp"

/**
 * ScheduleItem
 * @brief constructor for ScheduleItem class
 *
 * @param none
 * @return none
 */
ScheduleItem::ScheduleItem(){

}

/**
 * ~ScheduleItem
 * @brief deconstructor for ScheduleItem class
 *
 * @param none
 * @return none
 */
ScheduleItem::~ScheduleItem(){

}

/**
 * setState
 * @brief setter for the state variable
 *
 * @param state a state variable with the schedule state
 * @return none
 */
void ScheduleItem::setState(State* state){
	this->state = state;
}

/**
 * setTime
 * @brief setter for the time variable
 *
 * @param time a tm variable with the schedule time
 * @return none
 */
void ScheduleItem::setTime(tm* time){
	this->time = time;
}

/**
 * sendSchedule
 * @brief makes an api call scheduling the specified light/group id at the stored time with the stored state
 *
 * @param id an integer representing the id to schedule
 * @param group an integer (0 or 1) representing if we are scheduling a light or group, respectively
 * @return none
 */
void ScheduleItem::sendSchedule(int id, int group, std::string ip, std::string port){

    std::cout << "sending sch" << std::endl;


    Wt::WIOService *ioservice = new Wt::WIOService();
    ioservice->start();
    Wt::Http::Client *client = new Wt::Http::Client(*ioservice);
    client->done().connect(boost::bind(&ScheduleItem::handleHttpResponseSchedule, this, _1, _2));

    // Build the http message
    Wt::Http::Message message = Wt::Http::Message();
    message.addHeader("Content-type", "application/json");
    message.addBodyText(buildMessage(id, group));

    std::cout << buildMessage(id, group) << std::endl;

    client->post(ip + ":" + port + "/api/newdeveloper/schedules", message);
    ioservice->stop();
    delete ioservice;
}

/**
 * handleHttpResponseSchedule
 * @brief prints the response of sending the schedule
 *
 * @param err a boost error response variable
 * @param response a wt http response variable
 * @return none
 */
void ScheduleItem::handleHttpResponseSchedule(boost::system::error_code err, const Wt::Http::Message &response) {
    std::cout << "scheduled\nstatus: " +  std::to_string(response.status()) << std::endl;
    std::cout << "body: " +  response.body() << std::endl;
}

/**
 * buildMessage
 * @brief Takes the time and state and builds them into a message to send for scheduling
 *
 * @param id an integer representing the id of the scheduled item
 * @param group an integer (0 or 1) representing if we are scheduling a light or group, respectively
 * @return a string message to be the body of the schedule api call
 */
std::string ScheduleItem::buildMessage(int id, int group){

    std::string scheduleTime = "";
    scheduleTime += std::to_string(this->time->tm_year) + "-";
    scheduleTime += std::to_string(this->time->tm_mon) + "-";
    scheduleTime += std::to_string(this->time->tm_mday) + "T";
    
    if(std::to_string(this->time->tm_hour).size() == 1){
      scheduleTime += "0";
    }

    scheduleTime += std::to_string(this->time->tm_hour) + ":";
    if(std::to_string(this->time->tm_min).size() == 1){
      scheduleTime += "0";
    }

    scheduleTime += std::to_string(this->time->tm_min);
    scheduleTime += ":00";

  
    std::string onFlag = "false";
    if(this->state->isON()){
      onFlag = "true";
    }

    std::string buildName = "";
    if(group == 1){
        buildName += "G";
    } else{
        buildName += "L";
    }
    buildName += std::to_string(id) + " ";
    buildName += scheduleTime + " ";
    buildName += onFlag + " ";

    std::string dest = "lights";
    if(group == 1){
        dest = "groups";
    }

    return (
    "{\"name\": \"" + buildName + "\", " +
      "\"command\": {" + 
        "\"address\": \"/api/newdeveloper/" + dest + "/" + std::to_string(id)+ "/action\", " +
        "\"method\": \"PUT\", " +
        "\"body\": {" +
          "\"on\": "  + onFlag + ", " +
          "\"bri\": "  + std::to_string(this->state->getBrightness()) + ", " + 
          "\"hue\": "  + std::to_string(this->state->getHue()) + ", " +
          "\"transitiontime\": "  + std::to_string(this->state->getTransitionTime()) +
          "}" +
        "}, " +
      "\"time\": \"" + scheduleTime + "\"" +
    "}"
    );
}
