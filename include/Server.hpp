/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brhajji- <brhajji-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 06:27:20 by brhajji-          #+#    #+#             */
/*   Updated: 2023/01/07 16:12:40 by brhajji-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# define MAX_USERS 10

# include <iostream>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <unistd.h>
# include <vector>
# include <poll.h>
# include "signal.h"
# include "User.hpp"

class Server
{
	private:
		std::vector<pollfd> fds;
		std::vector<User *>	users;
		int					server_socket;
		int 				portNum;
		socklen_t			size;
	public:
		Server(/* args */);
		~Server();
		void init();
		void new_users();
};

Server::Server() : portNum(1500)
{
}

Server::~Server()
{
}

void Server::new_users()
{
	struct sockaddr_in	server_addr;
	size = sizeof(server_addr);
	int client = accept(server_socket, (struct sockaddr *)&server_addr,&size);
	if (client < 0)
		return ;
	users.push_back(new User(server_addr, client));
	fds.push_back(pollfd());
	fds.back().fd = client;
	fds.back().events = POLLIN;
		
	/* ---------------Reponse au client ------------------ */
	
	int bufsize = 1024;
	char buffer[bufsize];
	bool isExit;
	if (client > 0)
	{
		strcpy(buffer, "ON EST LAAAA \n");
		send(client, buffer, bufsize, 0);
	}

	/* ---------------Affichage donnee clients sur srv------------------ */
	do {
		recv(client, buffer, bufsize, 0); // recevoir des donnees
		std::cout<<"buffer" << buffer << " ";
		if (strcmp(buffer, "CAP LS"))
			std::cout<<"it's ok"<<std::endl;
		if (buffer[0] == '#') {
			*buffer = '*';
			isExit = true;
		}
	} while (*buffer != '*');
	do {
		std::cout << "\nServer: ";
		do {
			std::cin >> buffer;
			send(client, buffer, bufsize, 0);
			if (*buffer == '#') {
				send(client, buffer, bufsize, 0);
				*buffer = '*';
				isExit = true;
			}
		} while (*buffer != '*');
		std::cout << "Client: ";
		do {
			recv(client, buffer, bufsize, 0);
			std::cout << buffer << " ";
			if (*buffer == '#') {
				*buffer = '*';
				isExit = true;
			}
		} while (*buffer != '*');
	} while (!isExit);
		//test
		//close(client);
}

void Server::init()
{
	struct sockaddr_in	server_addr;
	/* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
	server_socket = socket(AF_INET, SOCK_STREAM, 0);//AF_INET => adresse ip v4 || sock_stream protocole tcp
	if (server_socket < 0) 
	{
		std::cout << "\nError establishing socket..." << std::endl;
		exit(1);
	}
	std::cout << "\n=> Socket server has been created..." << std::endl;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY); //inet_addr("127.0.0.1")
	server_addr.sin_port = htons(portNum);
	if ((bind(server_socket, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) 
	{
		std::cout << "=> Error binding connection, the socket has already been established..." << std::endl;
		return ;
	}
	std::cout << "=> Looking for clients..." << std::endl;
	if (listen(server_socket, 1) < 0)
		return ;
	new_users();

	//fermeture test

	//close(server_socket);
}

#endif