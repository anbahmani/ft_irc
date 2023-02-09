/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahmani <abahmani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 13:48:15 by brhajji-          #+#    #+#             */
/*   Updated: 2023/02/09 01:51:14 by abahmani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Server.hpp"

bool g_signal = false;

void 	signal_handler(int) {
	g_signal = true;
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Error:\tSpecify port and password: ./irc <port> <password>" << std::endl;
		exit(EXIT_SUCCESS);
	}
	else
	{
		signal(SIGINT, signal_handler);
		Server server(av[1], av[2]);
		if (g_signal)
			return (1);
		server.BuildServer();
		//server.Running();
	}
	return (0);
	
} 

//RMRECONNS

/*

Probleme: 

	- On ne peut pas se connecter avec un mot de passe faux, mais on peut sans mdp.
	- La commande PART ne fonctionne pas
	- connexion avec plus de 10 utilisateurs
	- command op
	- command notice

*/