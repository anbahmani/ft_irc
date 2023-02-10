#include "../include/Channel.hpp"
#include "../include/Server.hpp"

Channel::Channel(Server *serv, const std::string& name) : name(name) {};

Channel::~Channel() {}

