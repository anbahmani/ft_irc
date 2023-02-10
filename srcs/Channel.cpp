#include "Channel.hpp"
#include "Server.hpp"

const std::string&  Channel::getName() const { return (this->name); }
const std::string&  Channel::getTopic() const { return (this->topic); }
const std::string&  Channel::getKey() const { return (this->key); }
bool                Channel::getMode(char mode) const 
{ 
    std::map<const char, bool>::const_iterator it = this->modes.find(mode);

	if (it == this->modes.end())
		return (false);
	return (it->second); 
}

const std::string		Channel::getModes() const
{
	std::string res;
	std::string params;

	for(std::map<const char, bool>::const_iterator it = modes.begin(); it != modes.end(); it++)
	{
		if (it->second)
        {
            res += it->first;
            if (it->first == LIMIT)
                params += " " + ft_itoa(limit);
            if (it->first == KEY)
                params += " " + key;
            
        }
    }    
	return (res + params);
}

const std::vector<User *>&    Channel::getUsers() const { return (this->users); }
int                 Channel::getNbUsers() const { return (this->users.size()); }
int                 Channel::getLimit() const { return (this->limit); }

void                Channel::addUser(User* user, const std::string& key)
{

    if (modes[KEY] && key != this->key)
    {
        user->print(reply_prefix(serv->getName(), ERR_BADCHANNELKEY, user->getNickname()) + name + " :Cannot join channel (+k)");
        return ;
    }
    if (modes[INVITE] && !isInvited(user))
    {
        user->print(reply_prefix(serv->getName(), ERR_INVITEONLYCHAN, user->getNickname()) + name + " :Cannot join channel (+i)");
        return ;
    }   
    //Send join to all channel's users
    user->addChannel(name);
    users.push_back(user);
	print_log("User " + user->getNickname() + " joining channel " + name);
    sendToUsers(":" + user->getFullname() + " JOIN " + name);
    
    
    if (topic.length())
        sendTopic(user);

    printUsers(user);
    if (user->isOperator())
        sendToUsers(":" + user->getFullname() + " MODE " + name + " +o " + user->getNickname() );
    
}

int                Channel::addOperator(User* user) 
{
    if (!user)
        return (1);
    operators.push_back(user);
    return (0);
}
int               Channel::addBan(User* user) 
{
    if (!user)
        return (1);
    banned.push_back(user);
    return (0);
}

void                Channel::setTopic(const std::string& topic){ this->topic = topic; } 

void                Channel::editTopic(User* editor, const std::string& topic)
{
    setTopic(topic);
    topic_editor = editor->getNickname();
    sendToUsers(":" + editor->getFullname() +  " TOPIC " + name + " " + topic);
}


int               Channel::setMode(char mode, const std::string& param)
{
    if (mode == BAN)
        return (addBan(serv->getUser(param)));
    if (mode == CHANOP)
    {
        return (addOperator(serv->getUser(param)));
    }
    modes[mode] = true; 
    if (mode == LIMIT)
        this->limit = atoi(param.c_str());
    if (mode == KEY)
    {
        this->key = param;
    }
    return (0);
}

int               Channel::unsetMode(char mode, const std::string& param)
{
    if (mode == BAN)
        return (removeBan(serv->getUser(param)));  
    if (mode == CHANOP)
        return (removeOperator(serv->getUser(param)));
    modes[mode] = false;  
    if (mode == LIMIT)
        this->limit = MAX_INT;
    if (mode == KEY)
        this->key = "";
    return (0);
}


void                Channel::changeModes(User *user, std::vector<std::string> params)
{
    std::string modestring = params[1];
    std::string res = modestring[0] == '-' ? "-" : "+";
    std::string new_params;

    params.erase(params.begin(), params.begin() + 2);
    std::vector<std::string>::iterator  args = params.begin();
    for (size_t i = 0; i < modestring.length(); i++)
    {
        if (!is_chan_mode(modestring[i]))
            continue ;
        if (modestring[0] == '-')
        {
            if (modestring[i] == BAN || modestring[i] == CHANOP) 
            {
                if (args != params.end())
                {
                    if (unsetMode(modestring[i], *args))
                        continue ;
                    new_params += " " + *args;
                    res += modestring[i];
                    args++;
                }
                continue ;
            }
            unsetMode(modestring[i]);
        }
        else
        {
            if (is_param_mode(modestring[i])) 
            {
                if (args != params.end())
                {
                    if (setMode(modestring[i], *args))
                        continue ;
                    new_params += " " + *args;
                    res += modestring[i];
                    args++;
                }
                continue ;
            }
            setMode(modestring[i]);
        }
        res += modestring[i];
    }
    if (res.length() > 1)
        sendToUsers(":" + user->getFullname() + " MODE " + name + " " + res + new_params);

}

void                Channel::kickUser(User *user, User *target, const std::string& message)
{
    for (std::vector<User *>::iterator it = users.begin(); it != users.end(); it++)
    {
        if (*it == target)
        {   
            sendToUsers(":" + user->getFullname()+ " KICK " + name + " " + target->getNickname() \
                            + " " + message);
            users.erase(it);
            target->removeChannel(name);
            break ;
        }
    }
    removeOperator(target);
    removeInvite(target);
}

void                Channel::removeUser(User *user, const std::string& message)
{
    for (std::vector<User *>::iterator it = users.begin(); it != users.end(); it++)
    {
        if (*it == user)
        {   
            sendToUsers(":" + user->getFullname()+ " PART " + name + " " + message);
            users.erase(it);
            user->removeChannel(name);
	        print_log("User " + user->getNickname() + " leaving channel " + name);
            break ;
        }
    }
    removeOperator(user);
    removeInvite(user);
}

int               Channel::removeOperator(User *user)
{
    if (!user)
        return (1); 
    for (std::vector<User *>::iterator it = operators.begin(); it != operators.end(); it++)
    {
        if (*it == user)
        {
            operators.erase(it);
            break ;
        }
    }
    return (0);
}

void                Channel::removeInvite(User *user)
{
    for (std::vector<User *>::iterator it = invites.begin(); it != invites.end(); it++)
    {
        if (*it == user)
        {
            invites.erase(it);
            break ;
        }
    }
}

int                   Channel::removeBan(User *user)
{
    if (!user)
        return (1);
    for (std::vector<User *>::iterator it = banned.begin(); it != banned.end(); it++)
    {
        if (*it == user)
        {
            banned.erase(it);
            break ;
        }
    }
    return (0);
}


void                Channel::sendToUsers(const std::string& message) const
{
    for (std::vector<User *>::const_iterator it = users.begin(); it != users.end(); it++)
    {
       (*it)->print(message);
    }
    
}

void                Channel::sendTopic(User *user) const
{
    std::string     res;
    if (topic.length())
        res = reply_prefix(serv->getName(), RPL_TOPIC, user->getFullname()) + name + " " + topic;
    else
        res = reply_prefix(serv->getName(), RPL_NOTOPIC, user->getFullname()) + name + " No topic is set";
    user->print(res);

}

void                Channel::printUsers(User *target) const
{
    std::string     res = reply_prefix(serv->getName(), RPL_NAMREPLY, target->getNickname()) + "= " + name + " :";
   
    for (std::vector<User *>::const_iterator it = users.begin(); it != users.end(); it++)
    {
        if (isOperator(*it))
            res += "@";
        res += (*it)->getNickname() + " ";
    }
    target->print(res);
    res = reply_prefix(serv->getName(), RPL_ENDOFNAMES, target->getNickname()) + name + " :End of NAMES list";
    target->print(res);
}

void                Channel::printInfos(User *user) const
{
    std::string res = reply_prefix(serv->getName(), RPL_LIST, user->getNickname());
    user->print(res + name + " " + ft_itoa(users.size()) + " :" + topic);
}

void                 Channel::printBanList(User *target) const
{
    std::string base = reply_prefix(serv->getName(), RPL_BANLIST, target->getNickname()) + name + " ";

    for (std::vector<User *>::const_iterator it = banned.begin(); it != banned.end(); it++)
    {
        target->print(base + (*it)->getNickname());
    }
    target->print(reply_prefix(serv->getName(), RPL_ENDOFBANLIST, target->getNickname()) + name + " :End of channel ban list");
}

int                 Channel::isUser(User *user) const
{
    for (std::vector<User *>::const_iterator it = users.begin(); it != users.end(); it++)
    {
        if (*it == user)
            return (1);
    }
    return (0);
}

bool                Channel::isOperator(User *user) const
{
    for (std::vector<User *>::const_iterator it = operators.begin(); it != operators.end(); it++)
    {
        if (*it == user)
            return (true);
    }
    return (false);
}

bool                Channel::isInvited(User *user) const
{
    for (std::vector<User *>::const_iterator it = invites.begin(); it != invites.end(); it++)
    {
        if (*it == user)
            return (true);
    }
    return (false);
}

bool                Channel::isBanned(User *user) const
{
    for (std::vector<User *>::const_iterator it = banned.begin(); it != banned.end(); it++)
    {
        if (*it == user)
            return (true);
    }
    return (false);
}

void                Channel::invite(User *user, const std::string& name)
{
    User* target = serv->getUser(name);
    
    if (!target)
        return ;
    invites.push_back(target);
    target->print(":" + user->getNickname() + " INVITE " + name + " " + this->name);

}

void                Channel::printWho(User* target) const
{
    std::string base = reply_prefix(serv->getName(), RPL_WHOREPLY, target->getNickname()) + name +  " ";

    for (std::vector<User *>::const_iterator it = users.begin(); it != users.end(); it++)
    {
		if (!(*it)->getMode(INVISIBLE) || ((*it)->getMode(INVISIBLE) && !sharing_channel(target, *it)))
        {
            if (isOperator(*it))
                base += "@";
            else
                base += "~";
            target->print(base + (*it)->getUsername() + " " + (*it)->getHostname() + " " + serv->getName()\
                        + " " + (*it)->getNickname() + " H :0 " + (*it)->getRealname().substr(1));
        }
    }
}

Channel::Channel(Ircserv*   serv, const std::string& name):
                name(name),
                limit(MAX_INT),
                serv(serv)
{
	print_log("Creating new channel " + name);
    modes['i'] = false;
	modes['k'] = false;
	modes['l'] = false;
	modes['m'] = false;
	modes['n'] = false;
	modes['s'] = false;
	modes['t'] = true;
}

Channel::~Channel()
{
    print_log("Destroying channel " + name);
}