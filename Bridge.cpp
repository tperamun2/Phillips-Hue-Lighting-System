
#include "Bridge.hpp"

Bridge::Bridge(std::string name, std::string location, std::string ip, std::string port){
	this->name = name;
	this->location = location;
	this->ip = ip;
	this->port = port;
}

std::string Bridge::getName(){
	return this->name;
}

std::string Bridge::getLocation(){
	return this->location;
}

std::string Bridge::getIP(){
	return this->ip;
}

std::string Bridge::getPort(){
	return this->port;
}

void Bridge::setName(std::string name){
	this->name = name;
}

void Bridge::setLocation(std::string location){
	this->location = location;
}

void Bridge::setIP(std::string ip){
	this->ip = ip;
}

void Bridge::setPort(std::string port){
	this->port = port;
}
