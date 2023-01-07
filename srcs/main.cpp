/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vahemere <vahemere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 15:48:34 by vahemere          #+#    #+#             */
/*   Updated: 2023/01/07 13:58:11 by vahemere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Header.hpp"
#include "../includes/Serveur.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Error:\t Specify port and password: ./irc <port> <password>" << std::endl;
		return (-1);
	}
	else
	{
		// Creation du socket 
		int sockfd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET -> domaine (Prend la valeur tcp\ip) | SOCK_STREAM -> type (tcp/ip) | dernier param (protocole donc pas utile ici)
		if (sockfd == -1)
		{
			std::cerr << "Error creation socket failed" << std::endl;
			return (-1);
		}
		
		// configuration socket
		sockaddr_in	config;
		
		config.sin_addr.s_addr = htonl(INADDR_ANY); // Permet d'attribuer l'ip au serveur (htonl permet de l'identifier - ipv4)
		config.sin_family = AF_INET;				// correspond a notre type (tcp\ip)
		config.sin_port = htons(atoi(av[1]));		// precision du port htons permet de renvoyer le nombre dans l'ordre des octets du reseaux TCP/IP
		
		// attribution des informations au socket
		if (bind(sockfd, (sockaddr*)&config, sizeof(config)) == -1)
		{
			std::cerr << "Error: bind failed" << std::endl;
			return (-1);
		}

		// Passage en mode ecoute du socket (pour etablir connexion)
		if (listen(sockfd, 10) == -1)				// 10 connexion peuvent etre mise en attente
		{
			std::cerr << "Error: listening" << std::endl;
			return (-1);
		}

		// Mise en attente de connexion
		struct sockaddr_in	server_addr;
		int size = sizeof(server_addr);
		
		int client = accept(sockfd, (sockaddr *)&server_addr, &size);
		
	}
}