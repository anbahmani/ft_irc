/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brhajji- <brhajji-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 06:27:10 by brhajji-          #+#    #+#             */
/*   Updated: 2023/01/17 15:31:01 by brhajji-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server(char *port, std::string pass) : _portNum(atoi(port)), _password(pass)  {}

Server::~Server() {}

struct sockaddr_in	Server::get_struct_sockaddr(void) const
{
	return (_server_addr);
}

int	Server::get_server_socket(void) const
{
	return (_server_socket);
}

void add_client(int server, int epoll_instance, int *num_sockets)
{
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
	char		buffer[1024];
	std::string	data;
	if (client < 0)
			std::cerr << "Error:\tConnection failed.";
		while (true)
		{
			if (recv(client, buffer, sizeof(buffer), 0) <= 0)
			{
				std::cout << "Connection closed by client." << std::endl;
				return ;
			}
			
			data = buffer;
		
			if (data.find("CAP LS") != std::string::npos) 
			{
				std::cout << "client => server: " << data << std::endl;
    			std::string response = "CAP * LS :multi-prefix\n";
    			send(client, response.c_str(), response.length(), 0);
    		}
			
			else if (data.find("PASS") != std::string::npos)
			{
				std::cout << "client => server: " << data << std::endl;
    			if (data.find("1234") != std::string::npos)
				{
    			    std::string response = ":localhost:1500 001 brhajji- :wsh\n";
    			    send(client, response.c_str(), response.length(), 0);
    			} 
				else
				{
    			    std::string response = "PASS rejected\n";
    			    send(client, response.c_str(), response.length(), 0);
    			}
			}
		}

	//On incremente notre nombre de client
  	(*num_sockets)++;
	(void) (*num_sockets);
}

void	Server::BuildServer()
{
	struct epoll_event server_event;
	char		buffer[1024];
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
	_server_addr.sin_port = htons(_portNum);
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
	while (1)
	{
		num_event = epoll_wait(rc, events, num_socket, -1);
		for (int i = 0; i < num_event; i++)
		{
			std::cout<<"event fd = > "<<events[i].data.fd<< " srver socket "<<_server_socket<<std::endl; 
			if (events[i].data.fd == _server_socket) //si quelqu'un veut se connecter au serveur
			{
				add_client(_server_socket, rc, &num_socket);
					/* Accepter le client, l'ajouter a l'instance epoll, num_sockets++ */
			}
			else //si l'event concerne un client qu'on a deja ajouter a epoll
			{
				//std::cerr<<"else isisisisisii"<<std::endl;
				recv(events[i].data.fd, buffer, sizeof(buffer), 0);
				// {
				// 	std::cout << "Connection closed by client." << std::endl;
				// 	return ;
				// }
				// else
				std::cout<<"buffer => "<<buffer<<std::endl;
			}
		}
    }
	(void) (num_socket);
}

void	Server::Running(void)
{
	char		buffer[1024];
	std::string	data;
	
	// std::cout << "ici" << std::endl;
	while (true)
	{
		_size = sizeof(_server_addr);
		_client = accept(_server_socket, (struct sockaddr*)&_server_addr, &_size);
		if (_client < 0)
			std::cerr << "Error:\tConnection failed.";
		while (true)
		{
			if (recv(_client, buffer, sizeof(buffer), 0) <= 0)
			{
				std::cout << "Connection closed by client." << std::endl;
				return ;
			}
			
			data = buffer;
		
			if (data.find("CAP LS") != std::string::npos) 
			{
				std::cout << "client => server: " << data << std::endl;
    			std::string response = "CAP * LS :multi-prefix\n";
    			send(_client, response.c_str(), response.length(), 0);
    		}
			
			else if (data.find("PASS") != std::string::npos)
			{
				std::cout << "client => server: " << data << std::endl;
    			if (data.find(_password) != std::string::npos)
				{
    			    std::string response = "PASS accepted\n";
    			    send(_client, response.c_str(), response.length(), 0);
    			} 
				else
				{
    			    std::string response = "PASS rejected\n";
    			    send(_client, response.c_str(), response.length(), 0);
    			}
			}
		}
	}
}