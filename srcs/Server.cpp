/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vahemere <vahemere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 06:27:10 by brhajji-          #+#    #+#             */
/*   Updated: 2023/02/10 03:42:35 by vahemere         ###   ########.fr       */
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

Server::Server(char *port, std::string pass) : _portNum(port), _password(pass), _name("42IRC")  {}

Server::~Server() {}

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
		std::cout<<"rec =>"<<it->second->getFd()<<std::endl;
		if (fd == it->second->getFd())
			return (it->second);	
	}
	std::cout<<"ici\n";
	return (NULL);
}

void Server::add_client(int server, int epoll_instance, int *num_sockets, epoll_event event)
{	
	//nb de char lu
	// int				rc;
	
	//Socket pour le client
	int           client;
	
	//Pour stocker l'adresse du client (necessaire pour la fonction accept())
	sockaddr_in   addr_client;
	
	//Pour stocker la size de l'adresse client
	socklen_t     addr_size = sizeof(addr_client);

	//pour specifier les events du client
	struct epoll_event client_event;

	//On cree le socket client avec la fonction accept
	client = accept(server, reinterpret_cast<sockaddr*>(&addr_client), &addr_size);

	//On specifie les events	
	client_event.events = EPOLLIN;
	client_event.data.fd = client;

	//On ajoute le client a epoll
	epoll_ctl(epoll_instance, EPOLL_CTL_ADD, client, &client_event);
	
	//On instancie un nouvel utilisateur

	(void)(event);
	// char		buffer[1024];
	// std::string	data;
	// int i = 0;
	std::cout<<"fd =>"<<client<<std::endl;
	if (client < 0)
	{
		std::cerr << "Error:\tsyscall failed.";
		return ;
	}
	fcntl(client, F_SETFL, O_NONBLOCK);
	// while (true)
	// {
	// 	if (client_event.events == EPOLLIN)
	// 	{
	// 		rc = recv(client, buffer, sizeof(buffer), 0);
	// 		if (rc <= 0)
	// 		{
	// 			std::cout << "Connection closed by client0." << std::endl;
	// 			return ;
	// 		}
	// 		buffer[rc] = 0;
	// 		data = buffer;

	// 		size_t pos = 0;
	// 		int x = 0;
	// 		while ((pos = data.find('\n')) != std::string::npos)
	// 		{
	// 			Command cmd(data.substr(0, pos - 1));
	// 			std::cout<<"<--------------------->"<<'\n'<<data<<"<--------------------->"<<'\n'<<'\n';
	// 			x = execute_cmd(cmd, user, event, epoll_instance);
	// 			data.erase(0, pos + 1);
	// 			if (cmd.getName().compare("PASS") == 0)
	// 				i = 1;
	// 			if(!x)
	// 				return (delete user);
	// 			else if(x == 2)
	// 				break ;
	// 		}
	// 		if(x == 2)
	// 			break ;
	// 	}
	// }
	// if (!i)
	// {
	// 	std::string response = "NO PASSWORD\n";
	// 	send(client, response.c_str(), response.length(), 0);
	// 	epoll_ctl(rc, EPOLL_CTL_DEL, client, &event);
	// 	delete user;
	// 	return ;
	// }
	//On prepare la reponse d'authentification
    
	// std::cout<<"rep = "<<response<<std::endl;
	

	//On incremente notre nombre de client
  	(*num_sockets)++;
	(void) (*num_sockets);
}

void	Server::BuildServer()
{
	struct epoll_event server_event;
	char		buffer[1024];
	int			tmp = 0;
	// Creation socket
	_server_socket = socket(AF_INET, SOCK_STREAM, 0);	//AF_INET => adresse ip v4 || sock_stream protocole tcp
	if (_server_socket < 0 || fcntl(_server_socket, F_SETFL, O_NONBLOCK) < 0) 
	{
		std::cout << "Error:\tCreation socket failed." << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "\n=> Socket server has been created." << std::endl;
	
	// Configuration socket
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_addr.s_addr = htons(INADDR_ANY);	//inet_addr("127.0.0.1")
	_server_addr.sin_port = htons(atoi(_portNum.c_str()));
	if ((bind(_server_socket, (struct sockaddr*)&_server_addr,sizeof(_server_addr))) < 0) 
	{
		std::cout << "Error:\tbinding connection failed." << std::endl;
		exit(EXIT_FAILURE);
	}
	
	// Passage mode ecoute du socket
	std::cout << "=> Socket now listening." << std::endl;
	if (listen(_server_socket, 1) < 0)
		exit(EXIT_FAILURE);
	
	int rc = epoll_create(5);
	memset(&server_event, 0, sizeof(server_event));
	server_event.data.fd = _server_socket;
	server_event.events = EPOLLIN;
	epoll_ctl(rc, EPOLL_CTL_ADD, _server_socket, &server_event);
	int num_socket = 1;
	int num_event;
	struct epoll_event events[100];
	std::string	str;
	size_t pos;
	int		state = 0;
	while (1)
	{
		num_event = epoll_wait(rc, events, num_socket, -1);
		for (int i = 0; i < num_event; i++)
		{
			//std::cout<<"event fd = > "<<events[i].data.fd<< " srver socket "<<_server_socket<<std::endl; 
			if (events[i].data.fd == _server_socket && events[i].events == EPOLLIN) //si quelqu'un veut se connecter au serveur
			{
				add_client(_server_socket, rc, &num_socket, events[i]);
			}
			else if (events[i].events == EPOLLIN)//si l'event concerne un client qu'on a deja ajouter a epoll
			{
				tmp = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
				if (tmp <= 0)
				{
					std::cout << "Connection closed by client2." << std::endl;
					//Suppresion du socket de l'epoll
					epoll_ctl(rc, EPOLL_CTL_DEL, events[i].data.fd, &events[i]);
					_users.erase(get_user_by_fd(events[i].data.fd)->getNickname());
					close(events[i].data.fd);
				}
				else
				{
					buffer[tmp] = 0;
					User *user = NULL;
					str = buffer;
					std::cout<<"<+++++++++++++++++++->"<<'\n'<<buffer<<"<+++++++++++++++++++>"<<'\n'<<'\n';
					if (!get_user_by_fd(events[i].data.fd))
					{
							user = new User(events[i].data.fd);
							state = 0;
					}
					else
					{
						state = 1;
						user = get_user_by_fd(events[i].data.fd);
					}
					while ((pos = str.find('\n')) != std::string::npos)
					{
						Command cmd(str.substr(0, pos - 1));
						execute_cmd(cmd, user, events[i], rc);
						str.erase(0, pos + 1);
					}
					if (!state)
					{
						_users.insert(std::pair<std::string, User *>(user->getNickname(), user));
					}
				}
			}
		}
    }
	(void) (num_socket);
}

int	Server::execute_cmd(Command cmd, User *user, struct epoll_event event, int rc)
{
	std::string response;
	// std::cout<<"test = "<<cmd.cmds.find(cmd.getName())->second<<'\n';
	// 		std::cout<<cmd.getName()<<'\n';
	// std::vector<std::string> param = cmd.getParameters();
	// for (std::vector<std::string>::iterator iter = param.begin(); iter < param.end(); iter++)
	// 	std::cout<<"param => "<<*iter<<"\n";
	// std::cout<<std::endl;
	switch (cmd.cmds.find(cmd.getName())->second)
	{
		case CAP:
			if (!(cmd.getParameters()[0].compare("LS")))
			{
				std::string response = "CAP * LS :multi-prefix\r\n";
				send(user->getFd(), response.c_str(), response.length(), 0);
				std::cout<<"response : "<<response<<std::endl;
			}
			if (!(cmd.getParameters()[0].compare("REQ")))
			{
				std::string response = "CAP * ACK multi-prefix\r\n";
				send(user->getFd(), response.c_str(), response.length(), 0);
				std::cout<<"response : "<<response<<std::endl;

    		}
			if (!(cmd.getParameters()[0].compare("END")))
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
				//std::cout << "client => server: " << data << std::endl;
				std::string response = "PASS rejected\r\n";
				send(user->getFd(), response.c_str(), response.length(), 0);
				epoll_ctl(rc, EPOLL_CTL_DEL, user->getFd(), &event);
				return 0;
			}
			break;
		case USER:
			user->setUsername(cmd.getParameters()[0]);
			user->setFullname(cmd.getFName());
			break;
		case NICK:
			//On check si le nickname est deja utilise
			if (_users.find(cmd.getParameters()[0]) != _users.end())
			{
				std::string response = "NickName already used.\r\n";
				std::cout<<response<<std::endl;
				send(user->getFd(), response.c_str(), response.length(), 0);
				//Suppresion du socket de l'epoll
				epoll_ctl(rc, EPOLL_CTL_DEL, user->getFd(), &event);
				close(user->getFd());
				delete user;
				return 0;
			}
			else
				user->setNickname(cmd.getParameters()[0]);
			break;
		case PRIVMSG:
				if (cmd.getParameters().size() > 1 && cmd.getParameters()[0][0] == '#')//msg to channel
				{
					std::cout<<"ici 0\n";
					sendToChan(cmd, user);
				}
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
		case PING:
				response = "PONG localhost:"+_portNum+"\r\n";
				send(user->getFd(), response.c_str(), response.length(), 0);
				std::cout<<"response : "<<response<<std::endl;
			break;
		case WHOIS:
				response = ":localhost 311 "+cmd.getParameters()[0]+' '+user->getNickname()+' '+user->getNickname()+" localhost * "+user->getFullname()+"\r\n";
				send(user->getFd(), response.c_str(), response.length(), 0);
				std::cout<<"response : "<<response<<std::endl;
			break;
		case PART:
		{
			std::string channel = cmd.getParameters()[0].c_str() + 1;
			std::cout << "The user " << user->getNickname() << " quits the channel " << channel << "\r\n";
			std::map<std::string, std::vector<User *> >::iterator it = channels.find(channel);
			if (it != channels.end()) { //channel exists
				// response = ":"+user->getNickname()+" PART #" + channel +' '+cmd.getMsg()+"\r\n";
				// send(user->getFd(), response.c_str(), response.length(), 0);
				sendToChan(cmd, user);
				std::vector<User *>::iterator it_vector_user = std::find(channels[channel].begin(), channels[channel].end(), user);
				// std::cout<<"response : "<<response<<std::endl;
				channels[channel].erase(it_vector_user);
				// for (it_vector_user = channels[channel].begin(); it_vector_user < channels[channel].end(); it_vector_user++) //send this part response to all the user in the channel
				// 	send((*it_vector_user)->getFd(), response.c_str(), response.length(), 0);
			}
			break ;
		}
		case QUIT: //logout
				std::cout << "Connection closed by "<<user->getNickname() <<'\n';
				epoll_ctl(rc, EPOLL_CTL_DEL, user->getFd(), &event);
				_users.erase(user->getNickname());
				close(event.data.fd);
			break ;
		case JOIN: 
		{//join channel and create the channel if it does not already exist
			std::string channel = cmd.getParameters()[0].c_str() + 1;
			std::cout << "The user " << user->getNickname() << " joins the channel " << channel << "\r\n";
			std::map<std::string, std::vector<User *> >::iterator it = channels.find(channel);
			if (it == channels.end()) 
			{ //channel not exist
				std::vector<User *> myVector;
				myVector.push_back(user);
				channels.insert(std::pair<std::string, std::vector<User *> >(channel, myVector));
			}
			else 
			{ //the channel already exists
				channels[channel].push_back(user);
			}
			sendToChan(cmd, user);
			break ;
		}
		case OPER:
		{
			std::string name = cmd.getParameters()[0];
			std::string pwd;
			User *other = NULL;
			
			name.erase(std::remove(name.begin(), name.end(), ':'), name.end());
			if (cmd.getNbParameters() == 2)
				pwd = cmd.getParameters()[1];
			else if (cmd.getNbParameters() < 2)
			{
				reply(-1, ERR_NEEDMOREPARAMS, user, &cmd, *this);
				break ;
			}
			if (!get_user_by_fd(event.data.fd)->getNickname().compare(name))
			{
				if (pwd.compare(IRCOpwd) != 0)
				{
					reply(-1, ERR_PASSWDMISMATCH, user, &cmd, *this);
					break ;
				}
				if (pwd.compare(IRCOpwd) == 0)
				{
					user->setMode("IRCOP", true);
					display("MODE " + user->getNickname() + " +o", user);
					reply(RPL_YOUREOPER, -1, user, &cmd, *this);
					break ;
				}
			}
			std::map<std::string , User * >::iterator it = _users.find(name);
			if (it != _users.end())
				other = it->second;
			else
			{
				reply(-1, ERR_NOSUCHNICK, user, &cmd, *this);
				break ;
			}
			if (pwd.compare(IRCOpwd) != 0)
			{
				reply(-1, ERR_PASSWDMISMATCH, user, &cmd, *this);
				break ;
			}
			if (pwd.compare(IRCOpwd) == 0)
			{
				other->setMode("IRCOP", true);
				display("MODE " + other->getNickname() + " +o", user);
				reply(RPL_YOUREOPER, -1, other, &cmd, *this);
			}
			break ;
		}
		case MODE:
		{
			std::string name = cmd.getParameters()[0];
			std::string mode = "";
			if (cmd.getNbParameters() > 1)
				mode = cmd.getParameters()[1];
			name.erase(std::remove(name.begin(), name.end(), '\n'), name.end());
			name.erase(std::remove(name.begin(), name.end(), '\r'), name.end());
			name.erase(std::remove(name.begin(), name.end(), ':'), name.end());
			
			if (cmd.getNbParameters() < 1)
			{
				reply(-1, ERR_NEEDMOREPARAMS, user, &cmd, *this);
				break ;
			}
			if (cmd.getNbParameters() == 1 && name.compare(user->getNickname()) == 0)
			{
				reply(RPL_UMODEIS, -1, user, &cmd, *this);
				break ;
			}
			if (name != user->getNickname())
			{
				reply(-1, ERR_USERSDONTMATCH, user, &cmd, *this);
				break ;
			}
			if (!mode.empty())
			{
				if (!check_mode(mode))
				{
					reply(-1, ERR_UMODEUNKNOWNFLAG, user, &cmd, *this);
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
	}
	return 1;
}

void Server::sendToChan(Command cmd, User *user)
{
	if (channels.find(cmd.getParameters()[0].c_str() + 1) != channels.end())
	{
		std::string response;
		std::string chan = cmd.getParameters()[0].c_str() + 1;
		std::map<std::string, std::vector<User *> >::iterator it = channels.find(chan);
		//check if the chan exist and if the user is on it
		if (it != channels.end() && std::find((*it).second.begin(), (*it).second.end(), user) != (*it).second.end())
		{
			for (std::vector<User *>::iterator it_vector_user = channels[chan].begin(); it_vector_user < channels[chan].end(); it_vector_user++)
			{
				if ((*it_vector_user)->getFd() != user->getFd() || !cmd.getName().compare("PART"))
				{
					if (!cmd.getName().compare("PRIVMSG"))
						response = ":"+user->getNickname()+" "+cmd.getName()+" #"+chan+' '+cmd.getMsg()+"\r\n";
					else
						response = ":"+user->getNickname()+" "+cmd.getName()+" #"+chan+' '+cmd.getMsg()+"\r\n";
					send((*it_vector_user)->getFd(), response.c_str(), response.length(), 0);			
				}
				std::cout<<"response : "<<response<<std::endl;
			}
		}
		return ;
	}
	reply(-1, ERR_CANNOTSENDTOCHAN, user, &cmd, (*this));
}