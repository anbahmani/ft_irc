/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vahemere <vahemere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 15:48:34 by vahemere          #+#    #+#             */
/*   Updated: 2023/01/06 18:24:18 by vahemere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Header.hpp"
#include "../includes/Serveur.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Error:\t Specify port and password: ./irc <port> <password>" << std::endl;
		return ;
	}
	else
	{
		int			sock;
		sockaddr_in	config;
		
		// Creation du socket 
		sock = socket(AF_INET, SOCK_STREAM, 0); // AF_INET -> domaine (Prend la valeur tcp\ip) | SOCK_STREAM -> type (tcp/ip) | dernier param (protocole donc pas utile ici)
		if (sock != -1)
		{
			// Parametrage de la socket pour configurer la connexion
			config.sin_addr.s_addr = htonl(INADDR_ANY); // Permet d'attribuer l'ip au serveur (htonl permet de l'identifier)
			config.sin_family = AF_INET;				// correspond a notre type (tcp\ip)
			config.sin_port = htons(atoi(av[1]));		// precision du port htons permet de renvoyer le nombre dans l'ordre des octets du reseaux TCP/IP	
		}
		
	}
}