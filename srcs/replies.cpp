/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahmani <abahmani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 11:59:10 by vahemere          #+#    #+#             */
/*   Updated: 2023/02/09 02:15:12 by abahmani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"

void	reply(int rplcode, int errcode, User *user, Server &server) // set rplcode or errcode to 0 if you dont have code to send.
{
	std::string err = "";
	std::string	rpl = "";
	std::string response;
	std::stringstream rplconvert;
	std::stringstream errconvert;
	
	rplconvert << rplcode;
	rplconvert << errcode;
	
	switch (rplcode)
	{
		case RPL_YOUREOPER:
			rpl = ": You are now an IRC operator";
		default:
			break;
	}
	
	switch (errcode)
	{
		case ERR_PASSWDMISMATCH:
			err = ": Password incorect";
			break;
		case ERR_CANNOTSENDTOCHAN:
			err = ": You have to join this channel to send an msg to it.";
			break;
		default:
			break;
	}

	if (rplcode != -1)
		response = ":localhost:" + server.getPortNum() + ' ' + rplconvert.str() + ' ' + user->getNickname() + rpl + " " + '\n';
	else if (errcode != -1)
		response = ":localhost:" + server.getPortNum() + ' ' + errconvert.str() + ' ' + user->getNickname() + err + " " + '\n';

	write(user->getFd(), response.c_str(), response.length());
	// send(user->getFd(), response.c_str(), response.length(), 0);
	return ;
}

void	display(std::string to_display, User *user)
{
	to_display += "\r\n";
	send(user->getFd(), to_display.c_str(), to_display.length(), 0);
}
