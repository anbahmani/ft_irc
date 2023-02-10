#include "../include/Channel.hpp"

Channel::Channel(Server *serv, const std::string& name) : _serv(serv), name(name), _l(false), _m(false), _v(false), _o(false) {
    (void)_serv;
}

Channel::~Channel() {}


//getters
std::vector<User *>    Channel::getUser(void) const
{
    return (this->users);
}

std::string             Channel::getModeChan(void) const
{
    std::string mode = "+";

    if (_l == true)
        mode += 'l';
    if (_m == true)
        mode += 'm';
    if (_v == true)
        mode += 'v';
    if (_o == true)
        mode += 'o';
    return (mode);
}

//setters
void    Channel::setModeChan(std::string mode, bool state)
{
    if (mode == "o")
        _o = state;
    if (mode == "m")
        _m = state;
    if (mode == "l")
        _l = state;
    if (mode == "v")
        _v = state;
}


void    Channel::addUser(User *user)
{
    users.push_back(user);
}
