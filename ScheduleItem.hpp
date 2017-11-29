/**
 * @brief A header for the scheduleitem class which represents a scheduled event to send to the hue api
 * @author Gurkiran Tatla
 * @author Jake Schindler
 * @author Justine Kim
 * @author Paul Salvatore
 * @author Timal Peramune
 */

#ifndef SCHEDULEITEM_H
#define SCHEDULEITEM_H

#include "State.hpp"
#include "Light.hpp"
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <Wt/WIOService>

class State;

class ScheduleItem {

	tm* time;
	State* state;

    public:
        ScheduleItem();

        virtual ~ScheduleItem();

		void setState(State* state);

		void setTime(tm* time);

        void sendSchedule(int id, int group, std::string ip, std::string port);

    private:
    	void handleHttpResponseSchedule(boost::system::error_code err, const Wt::Http::Message &response);
    	
    	std::string buildMessage(int id, int group);

};

#endif




