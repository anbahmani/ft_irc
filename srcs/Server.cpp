/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vahemere <vahemere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 06:27:10 by brhajji-          #+#    #+#             */
/*   Updated: 2023/01/13 03:34:52 by vahemere         ###   ########.fr       */
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

void	Server::BuildServer()
{
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
	
	_client_fds.push_back(pollfd());
	_client_fds[0].fd = _client;
	_client_fds[0].events = POLLIN;
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