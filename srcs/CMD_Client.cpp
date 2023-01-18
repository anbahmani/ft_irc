/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CMD_Client.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vahemere <vahemere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 20:16:24 by vahemere          #+#    #+#             */
/*   Updated: 2023/01/17 22:22:06 by vahemere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"

int	pass(char *buf, Server const &server, int client)
{
	std::string	buffer;
	std::string answer;

	buffer = buf;
	if (buffer.find("PASS") != std::string::npos)
	{
		if (buffer.find(server.getServerPassword()) != std::string::npos)
		{
			answer = "Password accepted\n";
			send(client, answer.c_str(), answer.length(), 0);
			return (1);
		}
		else
		{
			answer = "Password rejected\n";
			send(client, answer.c_str(), answer.length(), 0);
		}
	}
	return (0);
	
}