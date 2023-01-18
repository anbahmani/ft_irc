/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vahemere <vahemere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 13:48:15 by brhajji-          #+#    #+#             */
/*   Updated: 2023/01/17 20:34:05 by vahemere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Server.hpp"

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