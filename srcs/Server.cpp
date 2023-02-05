/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brhajji- <brhajji-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 06:27:10 by brhajji-          #+#    #+#             */
/*   Updated: 2023/02/03 22:30:37 by brhajji-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server(char *port, std::string pass) : _portNum(port), _password(pass)  {}

Server::~Server() {}

struct sockaddr_in	Server::get_struct_sockaddr(void) const
{
	return (_server_addr);
}

int	Server::get_server_socket(void) const
{
	return (_server_socket);
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
		Command cmd(data);
		// std::cout<<cmd.getName()<<'\n';
		// std::vector<std::string> param = cmd.getParameters();
		// for (std::vector<std::string>::iterator iter = param.begin(); iter < param.end(); iter++)
		// 	std::cout<<"param => "<<*iter<<'\n';
		std::cout<<"<--------------------->"<<'\n'<<buffer<<"<--------------------->"<<'\n'<<'\n';
		int x = execute_cmd(cmd, user, event, epoll_instance);
		if(!x)
			return ;
		else if(x == 2)
				break ;
	}
	//On prepare la reponse d'authentification
	std::string response = ":localhost:"+_portNum+' '+RPL_WELCOME+' '+(user->getNickname())+": Bienvenue sur Chat Irc\n";
    
	std::cout<<"rep = "<<response<<std::endl;
	
	send(client, response.c_str(), response.length(), 0);
	response = ":localhost:"+_portNum+" 002 : Your host is localhost, running version 1.\n";
	send(client, response.c_str(), response.length(), 0);
	response = ":localhost:"+_portNum+" 003 : This server was created 01/01/2023.\n";
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
			else //si l'event concerne un client qu'on a deja ajouter a epoll
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
					Command cmd(str);
					execute_cmd(cmd, get_user_by_fd(events[i].data.fd), events[i], rc);
				}
			}
		}
    }
	(void) (num_socket);
}

int	Server::execute_cmd(Command cmd, User *user, struct epoll_event event, int rc)
{
	std::string response;
		std::cout<<"test = "<<cmd.cmds.find(cmd.getName())->second<<'\n';
				std::cout<<cmd.getName()<<'\n';
		std::vector<std::string> param = cmd.getParameters();
		for (std::vector<std::string>::iterator iter = param.begin(); iter < param.end(); iter++)
			std::cout<<"param => "<<*iter<<'\n';
	switch (cmd.cmds.find(cmd.getName())->second)
	{
		case CAP:
			if (!cmd.getParameters()[0].compare("LS"))
			{
				std::string response = "CAP * LS :multi-prefix\n";
				send(user->getFd(), response.c_str(), response.length(), 0);
				std::cout<<"response : "<<response<<std::endl;
			}
			if (!cmd.getParameters()[0].compare("REQ"))
			{
				std::string response = "CAP * ACK multi-prefix\n";
				send(user->getFd(), response.c_str(), response.length(), 0);
								std::cout<<"response : "<<response<<std::endl;

    		}
			if (!cmd.getParameters()[0].compare("END"))
			{
				std::cout<<"resasdsadsa\n"<<std::endl;
				return 2;
			}
			break;
		case PASS:
			if ((cmd.getParameters()[0].compare("1234")))
			{
				//std::cout << "client => server: " << data << std::endl;
				std::string response = "PASS rejected\n";
				send(user->getFd(), response.c_str(), response.length(), 0);
				epoll_ctl(rc, EPOLL_CTL_DEL, user->getFd(), &event);
				return 0;
			}
			break;
		case NICK:
			//On check si le nickname est deja utilise
			if (_users.find(cmd.getParameters()[0]) != _users.end())
			{
				std::string response = "NickName already used.\n";
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
		case PING:
				response = "PONG localhost:"+_portNum;
				send(user->getFd(), response.c_str(), response.length(), 0);
				std::cout<<"response : "<<response<<std::endl;
			break;
		case PART:
			if (!cmd.getParameters()[0].compare("localhost")) //logout
			{
				std::cout << "Connection closed by "<<user->getNickname() <<'\n';
				epoll_ctl(rc, EPOLL_CTL_DEL, user->getFd(), &event);
				_users.erase(user->getNickname());
				close(event.data.fd);
			}
			break ;
		case QUIT: //logout
				std::cout << "Connection closed by "<<user->getNickname() <<'\n';
				epoll_ctl(rc, EPOLL_CTL_DEL, user->getFd(), &event);
				_users.erase(user->getNickname());
				close(event.data.fd);
			break ;
		default:
			break;
	}
	return 1;
}