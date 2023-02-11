/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brhajji- <brhajji-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 06:27:10 by brhajji-          #+#    #+#             */
/*   Updated: 2023/02/11 04:11:53 by brhajji-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include <sstream>
# define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)


# define PRINT_LOG(x) std::cout << "[\033[1;36m" << __FUNCTION__ << "\033[0m] line : "\
  << __LINE__ << " [" << x << "]" << std::endl;

# define PRINT_ERR(x) std::cerr << "[\033[1;31m" << __FUNCTION__ << "\033[0m] line : "\
  << __LINE__ << " [" << x << "]" << std::endl;

# define PRINT_WIN(x) std::cout << "[\033[1;32m" << __FUNCTION__ << "\033[0m] line : "\
  << __LINE__ << " [" << x << "]" << std::endl;

# define PRINT_FUNC() std::cout << "[\033[36m" << __FUNCTION__ << "\033[0m] "\
  << __FILENAME__ << std::endl;

Server::Server(char *port, std::string pass) : _server_socket(0), _portNum(port), _password(pass), _name(SERVER_NAME), _rc(0){
	for (int i = 0; i < 100; i++){
		_tmp_fds[i] = false;
	}
}

Server::Server(void) : _name(SERVER_NAME){}

Server::~Server() {
	std::cout << "-------------\nShutting down server.\n---------------------\n";
	if (_server_socket > 0)
		close(_server_socket);
	for (std::map<std::string, User *>::iterator it = _users.begin(); it != _users.end(); it++)
		delete it->second;
	for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
		delete it->second;
	if (_rc > 0)
		close(_rc);
}

Server &Server::operator=(const Server &server){
	if (this != &server){
		_users = server._users;
		channels = server.channels;
		_server_addr = server._server_addr;
		_server_socket = server._server_socket;
		_password = server._password;
		_portNum = server._portNum;
	}
	return (*this);
}

struct sockaddr_in	Server::get_struct_sockaddr(void) const
{
	return (_server_addr);
}

int	Server::get_server_socket(void) const
{
	return (_server_socket);
}

std::string	Server::getNameServer(void) const
{
	return (this->_name);
}

std::string	Server::getPortNum(void) const
{
	return (this->_portNum);
}

User				*Server::get_user_by_fd(int fd)
{
	for (std::map<std::string, User *>::iterator it = this->_users.begin(); it != _users.end(); it++)
	{
		if (fd == it->second->getFd())
			return (it->second);	
	}
	return (NULL);
}

void Server::add_user(int server, int epoll_instance, int *num_sockets, epoll_event event)
{	
	//Socket pour le user
	int           user;
	
	//Pour stocker l'adresse du user (necessaire pour la fonction accept())
	sockaddr_in   addr_user;
	
	//Pour stocker la size de l'adresse user
	socklen_t     addr_size = sizeof(addr_user);

	//pour specifier les events du user
	struct epoll_event user_event;

	//On cree le socket user avec la fonction accept
	user = accept(server, reinterpret_cast<sockaddr*>(&addr_user), &addr_size);

	//On specifie les events	
	user_event.events = EPOLLIN;
	user_event.data.fd = user;

	//On ajoute le user a epoll
	epoll_ctl(epoll_instance, EPOLL_CTL_ADD, user, &user_event);
	
	//On instancie un nouvel utilisateur

	(void)(event);
	std::cout<<"fd =>"<<user<<std::endl;
	if (user < 0)
	{
		std::cerr << "Error:\tsyscall failed.";
		return ;
	}
	fcntl(user, F_SETFL, O_NONBLOCK);
	//On incremente notre nombre de user
  	(*num_sockets)++;
	(void) (*num_sockets);
}

void 	Server::signal_handler(int) {
	g_signal = true;
}

void	Server::BuildServer()
{
	struct epoll_event server_event;
	char		buffer[1024];
	int			tmp = 0;
	//time_t		current = std::time(0);
	int			optval = 1;
	// Creation socket
	if ((_server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){	//AF_INET => adresse ip v4 || sock_stream protocole tcp
		std::cout << "Error:\tCreation socket failed." << std::endl;
		return ;
	}
	if (setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)) < 0){
		std::cout << "Error:\tCreation socket failed." << std::endl;
		return ;
	}
	if (_server_socket < 0 || fcntl(_server_socket, F_SETFL, O_NONBLOCK) < 0) 
	{
		std::cout << "Error:\tCreation socket failed." << std::endl;
		return ;
	}
	std::cout << "\n=> Socket server has been created." << std::endl;
	
	// Configuration socket
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_addr.s_addr = htons(INADDR_ANY);	//inet_addr("127.0.0.1")
	_server_addr.sin_port = htons(atoi(_portNum.c_str()));
	if ((bind(_server_socket, (struct sockaddr*)&_server_addr,sizeof(_server_addr))) < 0) 
	{
		std::cout << "Error:\tbinding connection failed." << std::endl;
		return ;
	}
	
	// Passage mode ecoute du socket
	std::cout << "=> Socket now listening." << std::endl;
	if (listen(_server_socket, _server_addr.sin_port) < 0)
		return ;
	
	this->_rc = epoll_create(100);
	memset(&server_event, 0, sizeof(server_event));
	server_event.data.fd = _server_socket;
	server_event.events = EPOLLIN;
	epoll_ctl(this->_rc, EPOLL_CTL_ADD, _server_socket, &server_event);
	int num_socket = 1;
	int num_event;
	struct epoll_event events[100];
	std::string	str;
	size_t pos;
	signal(SIGINT, Server::signal_handler);
	int		state = 0;
	while (!g_signal)
	{
		//pingAll();
		num_event = epoll_wait(this->_rc, events, num_socket, -1);
		for (int i = 0; i < num_event; i++)
		{
			if (events[i].data.fd == _server_socket && events[i].events == EPOLLIN) //si quelqu'un veut se connecter au serveur
			{
				add_user(_server_socket, this->_rc, &num_socket, events[i]);
			}
			else if (events[i].events == EPOLLIN)//si l'event concerne un user qu'on a deja ajouter a epoll
			{
				tmp = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
				if (tmp <= 0)
				{
					std::cout << "Connection closed by client." << std::endl;
					//Suppresion du socket de l'epoll
					epoll_ctl(this->_rc, EPOLL_CTL_DEL, events[i].data.fd, &events[i]);
					if (get_user_by_fd(events[i].data.fd))
						_users.erase(get_user_by_fd(events[i].data.fd)->getNickname());
					close(events[i].data.fd);
					num_event--;
				}
				else
				{
					buffer[tmp] = 0;
					User *user = NULL;
					str = buffer;
					int x = 1;
					std::cout<<"<+++++++++++++++++++->"<<'\n'<<buffer<<"<+++++++++++++++++++>"<<'\n'<<'\n';
					if (!get_user_by_fd(events[i].data.fd))
					{
						std::cout<<"fd non reconnu "<<events[i].data.fd<<"\n";
						user = new User(events[i].data.fd);
						state = 0;
					}
					else
					{
						state = 1;
						user = get_user_by_fd(events[i].data.fd);
					}
					if (str.find("\r\n") == std::string::npos)
					{
						Command cmd(str.substr(0, str.find('\n')));
						if(execute_cmd(cmd, user, events[i], this->_rc, &num_event) == 0)
							x = 0;
					}
					else
					{
						while ((pos = str.find('\n')) != std::string::npos)
						{
							Command cmd(str.substr(0, pos - 1));
							if(execute_cmd(cmd, user, events[i], this->_rc, &num_event) == 0)
								x = 0;
							str.erase(0, pos + 1);
						}
					}
					if (state == 0 && x == 0)
					{
						std::cout << "USER ADD" << std::endl;
						_users.insert(std::pair<std::string, User *>(user->getNickname(), user));
					}
					else if (x == -1)
						delete user;
				}
			}
		}
		// if (std::time(0) % 30 == 0)
		// {
		// 	std::cout<<"Activity check.\n";
		// 	pingAll();
		// 	current = std::time(0);
		// }
		// checkDeath(&num_event);
    }
	(void) (num_socket);
}

int	Server::execute_cmd(Command cmd, User *user, struct epoll_event event, int rc, int *num_event)
{
	std::string response;
	// std::cout<<"test = "<<cmd.cmds.find(cmd.getName())->second<<std::endl;;
	// 		std::cout<<cmd.getName()<<std::endl;
	// std::vector<std::string> param = cmd.getParameters();
	// for (std::vector<std::string>::iterator iter = param.begin(); iter < param.end(); iter++)
	// 	std::cout<<"param => "<<*iter<<"|"<<std::endl;
	// std::cout<<std::endl;
	if (cmd.cmds.find(cmd.getName()) == cmd.cmds.end())
		return 2;
	switch (cmd.cmds.find(cmd.getName())->second)
	{
		case CAP:
			if (!(cmd.getParameters()[0].compare("LS")))
			{
				std::string response = "CAP * LS :multi-prefix\r\n";
				send(user->getFd(), response.c_str(), response.length(), 0);
				std::cout<<"response : "<<response<<std::endl;
			}
			else if (!(cmd.getParameters()[0].compare("REQ")))
			{
				std::string response = "CAP * ACK multi-prefix\r\n";
				send(user->getFd(), response.c_str(), response.length(), 0);
				std::cout<<"response : "<<response<<std::endl;

    		}
			else if (!(cmd.getParameters()[0].compare("END")))
			{
					std::string response = ":localhost:"+_portNum+" 001 "+(user->getNickname())+": Bienvenue sur Chat Irc\r\n";
					send(user->getFd(), response.c_str(), response.length(), 0);
					response = ":localhost:"+_portNum+" 002 : Your host is localhost, running version 1.\r\n";
					send(user->getFd(), response.c_str(), response.length(), 0);
					response = ":localhost:"+_portNum+" 003 : This server was created 01/01/2023.\r\n";
					send(user->getFd(), response.c_str(), response.length(), 0);
					response = ":localhost:"+_portNum+" 004 "+user->getNickname()+"\r\n";
					send(user->getFd(), response.c_str(), response.length(), 0);
			}
			break ;
		case PASS:
			if ((cmd.getParameters()[0].compare(_password)))
			{
				std::string response = "PASS rejected\r\n";
				send(user->getFd(), response.c_str(), response.length(), 0);
				epoll_ctl(rc, EPOLL_CTL_DEL, user->getFd(), &event);
				close(user->getFd());
				return -1;
			}
			else{
				std::cout << _tmp_fds[user->getFd()];
				_tmp_fds[user->getFd()] = true;
			}
			break;
		case USER:
			user->setUsername(user->getNickname());
			user->setFullname(cmd.getFName());
			break;
		case NICK:
		{
			if (!_tmp_fds[user->getFd()])
			{
				std::string response = "PASS NEEDED\r\n";
				send(user->getFd(), response.c_str(), response.length(), 0);
				epoll_ctl(rc, EPOLL_CTL_DEL, user->getFd(), &event);
				close(user->getFd());
				return -1;
			}
			//On check si le nickname est deja utilise
			_tmp_fds[user->getFd()] = false;
			std::string tmp = cmd.getParameters()[0];
			while (_users.find(tmp) != _users.end())
				tmp.insert(0,"_");
			user->setNickname(tmp);
			return 0;
		}
		case PRIVMSG:
				if (cmd.getParameters().size() > 1 && cmd.getParameters()[0][0] == '#')//msg to channel
					sendToChan(cmd, user);
				else if (cmd.getParameters().size() > 1 && cmd.getParameters()[0][0] != '#') //msg to user
				{
					if (_users.find(cmd.getParameters()[0]) != _users.end())
					{
						response = ":"+user->getNickname()+" "+cmd.getParameters()[0]+' '+cmd.getMsg()+"\r\n";
						display(response, (_users.find(cmd.getParameters()[0])->second));
						display(response, user);
						std::cout<<"response =>"<<response<<std::endl;
					}
					else
					{
						response = ":localhost 401 "+user->getNickname()+" :No such Nickname\r\n";
						send((_users.find(cmd.getParameters()[0])->second)->getFd(), response.c_str(), response.length(), 0);
						std::cout<<"response =>"<<response<<std::endl;
					}
				}
			break;
		case NOTICE:
				if (cmd.getParameters().size() > 1 && cmd.getParameters()[0][0] == '#')//msg to channel
					sendToChan(cmd, user);
				else if (cmd.getParameters().size() > 1 && cmd.getParameters()[0][0] != '#') //msg to user
				{
					if (_users.find(cmd.getParameters()[0]) != _users.end())
					{
						response = ":"+user->getNickname()+" "+cmd.getParameters()[0]+' '+cmd.getMsg()+"\r\n";
						display(response, (_users.find(cmd.getParameters()[0])->second));
						//display(response, user);
						std::cout<<"response =>"<<response<<std::endl;
					}
				}
			break;
		case PING:
				user->setPing(std::time(0));
				response = "PONG localhost:"+_portNum+"\r\n";
				send(user->getFd(), response.c_str(), response.length(), 0);
				std::cout<<"response : "<<response<<std::endl;
			break;
		case PONG:
				user->setPong(std::time(0));
			break;
		case WHOIS:
				response = ":localhost 311 "+cmd.getParameters()[0]+' '+user->getNickname()+' '+user->getNickname()+" localhost * "+user->getFullname()+"\r\n";
				send(user->getFd(), response.c_str(), response.length(), 0);
				std::cout<<"response : "<<response<<std::endl;
			break;
		case PART:
		{
			std::string channel = cmd.getParameters()[0].c_str() + 1;
			std::map<std::string, Channel *>::iterator it = channels.find(channel);
			if (it != channels.end() && channels[channel]) {
				std::cout << "The user " << user->getNickname() << " quits the channel " << channel << "\r\n";
				user->setChannel("");
				sendToChan(cmd, user);
				channels[channel]->delUser(user);
				if (channels[channel]->getUser().size() < 1)
					channels.erase(channel);
				}
			
			break ;
		}
		case QUIT: //logout
				std::cout << "Connection closed by "<<user->getNickname() <<'\n';
				epoll_ctl(rc, EPOLL_CTL_DEL, user->getFd(), &event);
				_users.erase(user->getNickname());
				close(event.data.fd);
				(*num_event)--;
			break ;

		
		case JOIN: 
		{//join channel and create the channel if it does not already exist
			std::string channel = cmd.getParameters()[0].c_str() + 1;
			std::cout << "The user " << user->getNickname() << " joins the channel " << channel << "\r\n";
			std::map<std::string, Channel *>::iterator it = channels.find(channel);
			if (it == channels.end()) 
			{ //channel not exist
				channels.insert(std::pair<std::string, Channel *>(channel, new Channel(this, channel)));
				channels[channel]->addUser(user);
				user->setChannel(channel);
			}
			else 
			{ //the channel already exists
				channels[channel]->addUser(user);
				user->setChannel(channel);
			}
			sendToChan(cmd, user);
			break ;
		}

		case OPER:  // become IRCOp (admin IRC)
		{
			if (cmd.getNbParameters() < 2)
			{
				reply(-1, ERR_NEEDMOREPARAMS, user, &cmd, *this, NULL);
				break ;
			}
			std::string pwd = cmd.getParameters()[1];
			std::string name = cmd.getParameters()[0];
			// name.erase(std::remove(name.begin(), name.end(), '\n'), name.end());
			name.erase(std::remove(name.begin(), name.end(), ':'), name.end());
			std::map<std::string , User * >::iterator it = _users.find(name);
			User *other = NULL;

			if (it != _users.end())
				other = it->second;
			else
			{
				reply(-1, ERR_NOSUCHNICK, user, &cmd, *this, NULL);
				break ;
			}
			if (pwd.compare(IRCOpwd) != 0)
			{
				reply(-1, ERR_PASSWDMISMATCH, user, &cmd, *this, NULL);
				break ;
			}
			if (pwd.compare(IRCOpwd) == 0)
			{
				other->setMode("IRCOP", true);
				display("MODE " + other->getNickname() + " +o", user);
				reply(RPL_YOUREOPER, -1, other, &cmd, *this, NULL);
			}
			break ;
		}
		
		case MODE:
		{
			std::string name = cmd.getParameters()[0];
			std::string channel = "";
			std::string mode = "";
			std::string who = "";
			Channel		*chan = NULL;
			User		*other = NULL;
			
			if (cmd.getNbParameters() < 1) // mandatory check
			{
				reply(-1, ERR_NEEDMOREPARAMS, user, &cmd, *this, NULL);
				break ;
			}
			else if (cmd.getNbParameters() > 1)
				mode = cmd.getParameters()[1];
			name.erase(std::remove(name.begin(), name.end(), '\n'), name.end());
			name.erase(std::remove(name.begin(), name.end(), '\r'), name.end());
			name.erase(std::remove(name.begin(), name.end(), ':'), name.end());
			
			if (name[0] == '#') // check if channel mode
			{
				channel = name.erase(0, 1);
				std::map<std::string, Channel *>::iterator it = channels.find(channel);
				if (it != channels.end())
					chan = it->second;
				else
				{
					reply(-1, ERR_NOSUCHCHANNEL, user, &cmd, *this, NULL);
					break ;
				}
				if (cmd.getNbParameters() > 1)
					mode = cmd.getParameters()[1];
				else
				{
					// std::cout << cmd
					reply(RPL_CHANNELMODEIS, -1, user, &cmd, *this, chan);
					break ;
				}

// set operator channel mode				
				if (mode.find("+o") != std::string::npos || mode.find("-o") != std::string::npos)
				{
					if (cmd.getNbParameters() > 2)
						who = cmd.getParameters()[2];
					else
						break ;
					if (mode.find("+") != std::string::npos)
					{
						std::map<std::string , User * >::iterator itt = _users.find(who);
						if (itt != _users.end())
							other = itt->second;
						else
							break ;
						if (user->getIRCOp() == true || user->get_op_chan() == true)
						{
							chan->addOpChan(other);
							display(":" + user->getNickname() + " MODE " + cmd.getParameters()[0] + " " + mode, other);
							display(":" + user->getNickname() + " MODE " + cmd.getParameters()[0] + " " + mode, user);
							break ;
						}
						else
						{
							reply(-1, ERR_CHANOPRIVISNEEDED, user, &cmd, *this, chan);
							break ;
						}
					}
					else
					{
						std::map<std::string , User * >::iterator itt = _users.find(who);
						if (itt != _users.end())
							other = itt->second;
						else
							break ;
						if (user->getIRCOp() == true || user->get_op_chan() == true)
						{
							chan->removeOpChan(other);
							std::cout << cmd.getParameters()[0] << std::endl;
							display(":" + user->getNickname() + " MODE " + cmd.getParameters()[0] + " " + mode, other);
							display(":" + user->getNickname() + " MODE " + cmd.getParameters()[0] + " " + mode, user);
							break ;
						}
						else
						{
							reply(-1, ERR_CHANOPRIVISNEEDED, user, &cmd, *this, chan);
							break ;
						}
					}
				}

				if (mode.find("+m") != std::string::npos || mode.find("-m") != std::string::npos)
				{
					if (mode.find("+") != std::string::npos)
					{
						if (user->getIRCOp() == true || user->get_op_chan() == true)
						{
							chan->addMChan();
							display(":" + user->getNickname() + " MODE " + cmd.getParameters()[0] + " " + mode, user);
							break ;
						}
						else
						{
							reply(-1, ERR_CHANOPRIVISNEEDED, user, &cmd, *this, chan);
							break ;
						}
					}
					else
					{
						if (user->getIRCOp() == true || user->get_op_chan() == true)
						{
							chan->removeMChan();
							display(":" + user->getNickname() + " MODE " + cmd.getParameters()[0] + " " + mode, user);
							break ;
						}
						else
						{
							reply(-1, ERR_CHANOPRIVISNEEDED, user, &cmd, *this, chan);
							break ;
						}
					}
				}

				if (mode.find("+v") != std::string::npos || mode.find("-v") != std::string::npos)
				{
					if (cmd.getNbParameters() > 2)
						who = cmd.getParameters()[2];
					else
						break ;
					if (mode.find("+") != std::string::npos)
					{
						std::map<std::string , User * >::iterator itt = _users.find(who);
						if (itt != _users.end())
							other = itt->second;
						else
							break ;
						if (user->getIRCOp() == true || user->get_op_chan() == true)
						{
							chan->addVChan(other);
							display(":" + user->getNickname() + " MODE " + cmd.getParameters()[0] + " " + mode, other);
							display(":" + user->getNickname() + " MODE " + cmd.getParameters()[0] + " " + mode, user);
							break ;
						}
						else
						{
							reply(-1, ERR_CHANOPRIVISNEEDED, user, &cmd, *this, chan);
							break ;
						}
					}
					else
					{
						std::map<std::string , User * >::iterator itt = _users.find(who);
						if (itt != _users.end())
							other = itt->second;
						else
							break ;
						if (user->getIRCOp() == true || user->get_op_chan() == true)
						{
							chan->removeVChan(other);
							display(":" + user->getNickname() + " MODE " + cmd.getParameters()[0] + " " + mode, other);
							display(":" + user->getNickname() + " MODE " + cmd.getParameters()[0] + " " + mode, user);
							break ;
						}
						else
						{
							reply(-1, ERR_CHANOPRIVISNEEDED, user, &cmd, *this, chan);
							break ;
						}
					}
				}
				
			}
			if (cmd.getNbParameters() == 1 && name.compare(user->getNickname()) == 0)
			{
				std::cout << user->getMode() << std::endl;
				reply(RPL_UMODEIS, -1, user, &cmd, *this, NULL);
				break ;
			}
			if (name != user->getNickname())
			{
				reply(-1, ERR_USERSDONTMATCH, user, &cmd, *this, NULL);
				break ;
			}
			if (!mode.empty())
			{
				if (!check_mode(mode))
				{
					reply(-1, ERR_UMODEUNKNOWNFLAG, user, &cmd, *this, NULL);
					break ;
				}
				else
				{
					if (check_if_mode(user, mode))
					{
						display("MODE " + user->getNickname() + " " + mode, user);
						break ;
					}
					if (check_if_not_mode(user, mode))
					{
						display("MODE " + user->getNickname() + " " + mode, user);
						break ;
					}
				}
			}
					
			break ;
		}
		case INVITE:
		{
			std::string name;
			std::string channel;
			User *other = NULL;
			
			if (cmd.getNbParameters() < 2)
			{
				reply(-1, ERR_NEEDMOREPARAMS, user, &cmd, *this, NULL);
				break ;
			}
			name = cmd.getParameters()[0];
			name.erase(std::remove(name.begin(), name.end(), ':'), name.end());
			channel = cmd.getParameters()[1].erase(0, 1);
			std::map<std::string , User * >::iterator it = _users.find(name);
			std::map<std::string, Channel *>::iterator itt = channels.find(channel);
			if (it != _users.end())
				other = it->second;
			if (itt == channels.end())
			{
				reply(-1, ERR_NOSUCHCHANNEL, user, &cmd, *this, NULL);
				break ;
			}
			if (it == _users.end())
			{
				reply(-1, ERR_NOSUCHNICK, user, &cmd, *this, NULL);
				break ;
			}
			if (user->getChannel() != channel)
			{
				reply(-1, ERR_NOTONCHANNEL, user, &cmd, *this, NULL);
				break ;
			}
			if (other)
			{
				if (other->getChannel() == channel)
				{
					reply(-1, ERR_USERONCHANNEL, user, &cmd, *this, NULL);
					break ;
				}
				if (other->get_i() == true && user->getIRCOp() == false)
				{
					reply(-1, ERR_CHANOPRIVISNEEDED, user, &cmd, *this, NULL);
					break ;
				}
				else
				{
					reply(RPL_INVITING, -1, user, &cmd, *this, NULL);
					display(":" + user->getNickname() + " INVITE " + other->getNickname() + " #" + channel, other);
				}
			}
			break ;
		}
		case KILL:
		{
			if (cmd.getNbParameters() < 1){
				reply(-1, ERR_NEEDMOREPARAMS, user, &cmd, *this, NULL);
				break;
			}
			if (!user->getIRCOp()){
				reply(-1, ERR_NOPRIVILEGES, user, &cmd, *this, NULL);
				break;
			}
			std::string user_to_kill_name = cmd.getParameters()[0];
			std::map<std::string, User *>::iterator it_vector_user = _users.find(user_to_kill_name);
			if (it_vector_user == _users.end() || !_users[user_to_kill_name]){
				reply(-1, ERR_NOSUCHNICK, user, &cmd, *this, NULL);
				break;
			}
			User *target = this->_users[user_to_kill_name];
			std::map<std::string, Channel *>::iterator it_map_chann;
			it_map_chann = channels.begin();
			while ( it_map_chann != channels.end()){
				std::vector<User *> vector_user = it_map_chann->second->getUser();
				std::vector<User *>::iterator it_vector_users = std::find(vector_user.begin(), vector_user.end(), target);
				if (it_vector_users != vector_user.end())
					vector_user.erase(it_vector_users);
				if (vector_user.size() < 1){
					std::map<std::string, Channel *>::iterator it_tmp = it_map_chann;
					it_map_chann++;
					delete it_tmp->second;
					channels.erase(it_tmp);
				}
				else
					it_map_chann++;
			}
			close(target->getFd());
			_users.erase(user_to_kill_name);
			std::cout << "The user " << user->getNickname() << " has killed the user" << user_to_kill_name << "\r\n";
			if (cmd.getNbParameters() > 1 && cmd.getMsg().size() > 1)
				target->setComment(cmd.getMsg());
			delete target;
			break ;
		}
	}
	(void)(*num_event);
	return 1;
}

void Server::sendToChan(Command cmd, User *user)
{
	if (channels.find(cmd.getParameters()[0].c_str() + 1) != channels.end())
	{
		std::string response;
		std::string chan = cmd.getParameters()[0].c_str() + 1;
		std::map<std::string, Channel *>::iterator it = channels.find(chan);
		//check if the chan exist and if the user is on it
		if (it != channels.end() && channels[chan])
		{
			std::vector<User *> vector_user = channels[chan]->getUser();
			if (std::find(vector_user.begin(), vector_user.end(), user) != vector_user.end())
			{	
				if (!cmd.getName().compare("PRIVMSG") && channels[chan]->getModeChan().find('m') != std::string::npos)
				{
					std::vector<User *> vec = channels[chan]->getOpUser();
					if (!user->get_v_chan() && !user->getIRCOp() && std::find(vec.begin(), vec.end(), user) == vec.end())
						return reply(-1, ERR_CANNOTSENDTOCHAN, user, &cmd, (*this), channels[cmd.getParameters()[0].c_str() + 1]);
				}
				for (std::vector<User *>::iterator it_vector_user = vector_user.begin(); it_vector_user < vector_user.end(); it_vector_user++)
				{
					if ((*it_vector_user)->getFd() != user->getFd() || !cmd.getName().compare("PART"))
					{
						if (!cmd.getName().compare("PRIVMSG"))
							response = ":"+user->getNickname()+"@localhost "+cmd.getName()+" #"+chan+' '+cmd.getMsg()+"\r\n";
						else
							response = ":"+user->getNickname()+"@localhost "+cmd.getName()+" #"+chan+' '+cmd.getMsg()+"\r\n";
						send((*it_vector_user)->getFd(), response.c_str(), response.length(), 0);			
					}
					std::cout<<"response : "<<response<<std::endl;
				}
			}
		}
		return ;
	}
	return (reply(-1, ERR_CANNOTSENDTOCHAN, user, &cmd, (*this), channels[cmd.getParameters()[0].c_str() + 1]));
}

void Server::checkDeath(int *num_event)
{
	std::string str;
	User	*tmp;
	for (std::map<std::string, User *>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second->getPong() + 60 < std::time(0) && it->second->getPing() + 60 < std::time(0))
		{
			tmp = it->second;
			std::cout << "Connection lost by "<< tmp->getNickname()<< std::endl;
			//Suppresion du socket de l'epoll
			epoll_ctl(this->_rc, EPOLL_CTL_DEL, tmp->getFd(), NULL);
			close(tmp->getFd());
			_users.erase(tmp->getNickname());
			delete tmp;
			(*num_event)--;
		}
	}
	(void)(num_event);
}

void Server::pingAll()
{
	std::string str;
	std::stringstream ss;
	for (std::map<std::string, User *>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second->getPing() + 30 < std::time(0))
		{
			ss<<std::time(0);
			str = "PING "+ss.str()+" localhost:"+_portNum+"\r\n";
			send(it->second->getFd(), str.c_str(), str.length(), 0);
			std::cout<<"response : "<<str<<std::endl;
		}
	}
}