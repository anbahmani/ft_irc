/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brhajji- <brhajji-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 06:27:10 by brhajji-          #+#    #+#             */
/*   Updated: 2023/02/07 19:02:00 by brhajji-         ###   ########.fr       */
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
	std::map<std::string, User*>::iterator it;
	for (it = this->_users.begin(); it != _users.end(); it++)
	{
		if (fd == it->second->getFd())
			return (it->second);	
	}
	return (NULL);
}

void Server::add_client(int server, int epoll_instance, int *num_sockets, epoll_event event)
{	
	//nb de char lu
	int				rc;
	
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
	User *user = new User(client);
	
	char		buffer[1024];
	std::string	data;
	int i = 0;
	if (client < 0)
			std::cerr << "Error:\tConnection failed.";
	while (true)
	{
		rc = recv(client, buffer, sizeof(buffer), 0);
		if (rc <= 0)
		{
			std::cout << "Connection closed by client." << std::endl;
			return ;
		}
		buffer[rc] = 0;
		data = buffer;

		size_t pos = 0;
		int x = 0;
		while ((pos = data.find('\n')) != std::string::npos)
		{
			Command cmd(data.substr(0, pos - 1));
			std::cout<<"<--------------------->"<<'\n'<<data<<"<--------------------->"<<'\n'<<'\n';
			x = execute_cmd(cmd, user, event, epoll_instance);
			data.erase(0, pos + 1);
			if (cmd.getName().compare("PASS") == 0)
				i = 1;
			if(!x)
				return (delete user);
			else if(x == 2)
				break ;
		}
		if(x == 2)
			break ;
	}
	if (!i)
	{
		std::string response = "NO PASSWORD\n";
		send(client, response.c_str(), response.length(), 0);
		epoll_ctl(rc, EPOLL_CTL_DEL, client, &event);
		delete user;
		return ;
	}
	//On prepare la reponse d'authentification
	std::string response = ":localhost:"+_portNum+" 001 "+(user->getNickname())+": Bienvenue sur Chat Irc\n";
    
	std::cout<<"rep = "<<response<<std::endl;
	
	send(client, response.c_str(), response.length(), 0);
	response = ":localhost:"+_portNum+" 002 : Your host is localhost, running version 1.\r\n";
	send(client, response.c_str(), response.length(), 0);
	response = ":localhost:"+_portNum+" 003 : This server was created 01/01/2023.\r\n";
	send(client, response.c_str(), response.length(), 0);
	response = ":localhost:"+_portNum+" 004 "+user->getNickname()+"\r\n";
	send(client, response.c_str(), response.length(), 0);
	
	//On ajoute le User a la map
	_users.insert(std::pair<std::string, User *>(user->getNickname(), user));
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
	if (_server_socket < 0) 
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
	while (1)
	{
		num_event = epoll_wait(rc, events, num_socket, -1);
		for (int i = 0; i < num_event; i++)
		{
			//std::cout<<"event fd = > "<<events[i].data.fd<< " srver socket "<<_server_socket<<std::endl; 
			if (events[i].data.fd == _server_socket) //si quelqu'un veut se connecter au serveur
			{
				add_client(_server_socket, rc, &num_socket, events[i]);
				/* Accepter le client, l'ajouter a l'instance epoll, num_sockets++ */
			}
			else if (events[i].events & EPOLLIN)
			//else //si l'event concerne un client qu'on a deja ajouter a epoll
			{
				tmp = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
				if (tmp <= 0)
				{
					std::cout << "Connection closed by client." << std::endl;
					//Suppresion du socket de l'epoll
					epoll_ctl(rc, EPOLL_CTL_DEL, events[i].data.fd, &events[i]);
					_users.erase(get_user_by_fd(events[i].data.fd)->getNickname());
					close(events[i].data.fd);
				}
				else
				{
					buffer[tmp] = 0;
					str = buffer;
					std::cout<<"<+++++++++++++++++++->"<<'\n'<<buffer<<"<+++++++++++++++++++>"<<'\n'<<'\n';
					while ((pos = str.find('\n')) != std::string::npos)
					{
						Command cmd(str.substr(0, pos - 1));
						execute_cmd(cmd, get_user_by_fd(events[i].data.fd), events[i], rc);
						str.erase(0, pos + 1);
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
				std::cout<<"resasdsadsa\n"<<std::endl;
				return 2;
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
					// if (_users.find(cmd.getParameters()[0]) != _users.end())
					// {
						// response = ":"+user->getNickname()+" PRIVMSG "+cmd.getParameters()[0]+' '+cmd.getMsg()+"\r\n";
						// std::cout<<"response : "<<response<<std::endl;
						
						response = ":"+user->getNickname()+" localhost PRIVMSG "+cmd.getParameters()[0]+' '+cmd.getMsg()+"\r\n";
						// send(user->getFd(), response.c_str(), response.length(), 0);
						send((_users.find(cmd.getParameters()[0])->second)->getFd(), response.c_str(), response.length(), 0);
						std::cout<<"response : "<<response<<std::endl;
					// }
					// else
					// {
					// 	response = ":localhost 401 "+user->getNickname()+" :No such Nickname";
					// 	send((_users.find(cmd.getParameters()[0])->second)->getFd(), response.c_str(), response.length(), 0);
					// 	std::cout<<"response : "<<response<<std::endl;
					// }
				}
				else if (cmd.getParameters().size() > 1 && cmd.getParameters()[0][0] != '#') //msg to user
				{
					if (_users.find(cmd.getParameters()[0]) != _users.end())
					{
						response = ":"+user->getNickname()+" PRIVMSG "+cmd.getParameters()[0]+' '+cmd.getMsg()+"\r\n";
						send((_users.find(cmd.getParameters()[0])->second)->getFd(), response.c_str(), response.length(), 0);
					//	write(user->getFd(), response.c_str(), response.length());
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
				response = ":localhost 311 "+cmd.getParameters()[0]+' '+user->getNickname()+' '+user->getUsername()+" localhost * "+user->getFullname()+"\r\n";
				send(user->getFd(), response.c_str(), response.length(), 0);
				std::cout<<"response : "<<response<<std::endl;
			break;
		case PART:
		{
			if (!cmd.getParameters()[0].compare("localhost")) //logout
			{
				std::cout << "Connection closed by "<<user->getNickname() <<'\n';
				epoll_ctl(rc, EPOLL_CTL_DEL, user->getFd(), &event);
				_users.erase(user->getNickname());
				close(event.data.fd);
			}
			std::string channel = cmd.getParameters()[0];
			std::cout << "The user " << user->getNickname() << " quits the channel " << channel << "/n";
			std::map<std::string, std::vector<User *> >::iterator it = channels.find(channel);
			if (it != channels.end()) { //channel exists
				std::vector<User *>::iterator it_vector_user = std::find(channels[channel].begin(), channels[channel].end(), user);
				channels[channel].erase(it_vector_user);
				response = ":myserv PART #" + channel;
				send(user->getFd(), response.c_str(), response.length(), 0);
				for (it_vector_user = channels[channel].begin(); it_vector_user < channels[channel].end(); it_vector_user++) //send this part response to all the user in the channel
					send((*it_vector_user)->getFd(), response.c_str(), response.length(), 0);
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
			std::string channel = cmd.getParameters()[0];
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
			response = ":myserv JOIN #" + channel;
			//send(user->getFd(), response.c_str(), response.length(), 0);
			for (std::vector<User *>::iterator it_vector_user = channels[channel].begin(); it_vector_user < channels[channel].end(); it_vector_user++) //send the join response to all the user in the channel
				send((*it_vector_user)->getFd(), response.c_str(), response.length(), 0);
			break ;
		}
		case OPER:  // become admin
			std::string pwd = cmd.getParameters()[1];
			
			if (cmd.getNbParameters() < 1)
				std::cout << ERR_NEEDMOREPARAMS << std::endl;
			if (pwd.compare(IRCOpwd) == 0)
			{
				user->setIRCOp(true);
				display("MODE " + user->getNickname() + " +o", user);
				reply(RPL_YOUREOPER, -1, user, (*this));
				
			}
			if (pwd.compare(IRCOpwd) != 0)
				reply(-1, ERR_PASSWDMISMATCH, user, (*this));
				
			break ;
	}
	return 1;
}