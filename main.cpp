/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahmani <abahmani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 13:48:15 by brhajji-          #+#    #+#             */
/*   Updated: 2023/02/10 04:15:18 by abahmani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Server.hpp"

bool g_signal = false;

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Error:\tSpecify port and password: ./irc <port> <password>" << std::endl;
		exit(EXIT_SUCCESS);
	}
	else
	{
		Server server(av[1], av[2]);
		server.BuildServer();
		//server.Running();
	}
	return (0);
	
} 

//RMRECONNS

/*

Probleme: 

	- La commande PART ne fonctionne pas
	- connexion avec plus de 10 utilisateurs
	- command op
	- command notice

*/