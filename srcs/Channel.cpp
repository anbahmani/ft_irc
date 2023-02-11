#include "../include/Channel.hpp"

Channel::Channel(Server *serv, const std::string& name) : _serv(serv), name(name), _l(false), _m(false), _v(false), _o(false) {
    (void)_serv;
}

Channel::~Channel() {}


//getters
std::vector<User *>    Channel::getUser(void) const
{
    return (users);
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

void    Channel::addOpChan(User *user)
{
    if (user->get_op_chan() == true)
        return ;
    else
    {
        user->setModeChan("o", true);
        operators.push_back(user);
    } 
}

void    Channel::addVChan(User *user)
{
    if (user->get_v_chan() == true)
        return ;
    else
    {
        user->setModeChan("v", true);
        v_speak.push_back(user);
    }
}

void    Channel::addMChan(void)
{
    _m = true;
    return ;
}

void    Channel::removeMChan(void)
{
    _m = false;
    return ;
}

void    Channel::removeOpChan(User *user)
{
    std::vector<User *>::iterator it = std::find(operators.begin(), operators.end(), user);
    if (user == *it)
    {
        user->setModeChan("o", false);
        operators.erase(it);
    }
}

void    Channel::removeVChan(User *user)
{
    std::vector<User *>::iterator it = std::find(v_speak.begin(), v_speak.end(), user);
    if (user == *it)
    {
        user->setModeChan("v", false);
        v_speak.erase(it);
    }
}

void    Channel::delUser(User *user)
{
    std::vector<User *>::iterator it_vector_user = std::find(users.begin(), users.end(), user);
    users.erase(it_vector_user);
}
