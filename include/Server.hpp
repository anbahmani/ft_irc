/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahmani <abahmani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 06:27:20 by brhajji-          #+#    #+#             */
/*   Updated: 2023/01/15 18:09:49 by abahmani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# define MAX_USERS 10

# include "irc.hpp"
# include "User.hpp"

class Server
{
	public:
	
		Server(char *port, std::string pass);
		~Server();
		
		void	BuildServer();
		void	Running();
		// void new_users();

		struct sockaddr_in	get_struct_sockaddr(void) const;
		int					get_server_socket(void) const;

	private:
	
		std::vector<pollfd> _client_fds;
		// std::vector<User *>	_users;
		struct sockaddr_in	_server_addr;
		int					_server_socket;
		int					_client;
		int 				_portNum;
		std::string			_password;
		socklen_t			_size;

};

// Commands

 /*
int		cap();
int		pass();
int		nick();
int		user();
int		oper();
int		mode();
int     quit();
int		msg();
int		join();
int		part();
int		ping();
int		pong();
int		topic();
int		list();
int		invite();
int		kick();
int		who();
int		kill();
int		wallops();
int		notice();
int		whois();
*/

// void Server::new_users()
// {
// 	struct sockaddr_in	server_addr;
// 	size = sizeof(server_addr);
// 	int client = accept(server_socket, (struct sockaddr *)&server_addr,&size);
// 	if (client < 0)
// 		return ;
// 	users.push_back(new User(server_addr, client));
// 	fds.push_back(pollfd());
// 	fds.back().fd = client;
// 	fds.back().events = POLLIN;
		
// 	/* ---------------Reponse au client ------------------ */
	
// 	int bufsize = 1024;
// 	char buffer[bufsize];
// 	bool isExit;
// 	if (client > 0)
// 	{
// 		strcpy(buffer, "ON EST LAAAA \n");
// 		send(client, buffer, bufsize, 0);
// 	}

// 	/* ---------------Affichage donnee clients sur srv------------------ */
// 	do {
// 		recv(client, buffer, bufsize, 0); // recevoir des donnees
// 		std::cout<<"buffer" << buffer << " ";
// 		if (strcmp(buffer, "CAP LS"))
// 			std::cout<<"it's ok"<<std::endl;
// 		if (buffer[0] == '#') {
// 			*buffer = '*';
// 			isExit = true;
// 		}
// 	} while (*buffer != '*');
// 	do {
// 		std::cout << "\nServer: ";
// 		do {
// 			std::cin >> buffer;
// 			send(client, buffer, bufsize, 0);
// 			if (*buffer == '#') {
// 				send(client, buffer, bufsize, 0);
// 				*buffer = '*';
// 				isExit = true;
// 			}
// 		} while (*buffer != '*');
// 		std::cout << "Client: ";
// 		do {
// 			recv(client, buffer, bufsize, 0);
// 			std::cout << buffer << " ";
// 			if (*buffer == '#') {
// 				*buffer = '*';
// 				isExit = true;
// 			}
// 		} while (*buffer != '*');
// 	} while (!isExit);
// 		//test
// 		//close(client);
// }

#endif