/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vahemere <vahemere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 11:59:10 by vahemere          #+#    #+#             */
/*   Updated: 2023/02/10 02:37:08 by vahemere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"

void	reply(int rplcode, int errcode, User *user, Command *cmd, Server &server) // set rplcode or errcode to 0 if you dont have code to send.
{
	std::string err = "";
	std::string	rpl = "";
	std::string response;
	std::stringstream rplconvert;
	std::stringstream errconvert;
	
	rplconvert << rplcode;
	errconvert << errcode;
	
	switch (rplcode)
	{
		case RPL_YOUREOPER:
			rpl = ": You are now an IRC operator";
			break ;
		case RPL_UMODEIS:
			rpl = ": Your user mode is +" + user->getMode();
			break ;
		default:
			break;
	}
	
	std::cout << errcode << std::endl;
	switch (errcode)
	{
		case ERR_PASSWDMISMATCH:
			err = " : Password incorect";
			break ;
		case ERR_NEEDMOREPARAMS:
			err = cmd->getName() + " : Not enough parameters";
			break ;
		case ERR_NOSUCHNICK:
			err = cmd->getParameters()[0] + " : No such nick/channel";
			break ;
		case ERR_USERSDONTMATCH:
			err = " : Cant change mode for other users";
			break ;
		case ERR_UMODEUNKNOWNFLAG:
			err = " : Unknown MODE flag";
			break ;
		case ERR_CANNOTSENDTOCHAN:
			err = " : Cannot send to channel";
		default:
			break ;
	}

	if (rplcode != -1)
		response = ":localhost:" + server.getPortNum() + ' ' + rplconvert.str() + ' ' + user->getNickname() + rpl + ' ' + '\n';
	else if (errcode != -1)
		response = ":localhost:" + server.getPortNum() + ' ' + errconvert.str() + ' ' + user->getNickname() + ' ' + err + ' ' + '\n';

	std::cout << response.c_str() << std::endl;
	write(user->getFd(), response.c_str(), response.length());
	// send(user->getFd(), response.c_str(), response.length(), 0);
	return ;
}

void	display(std::string to_display, User *user)
{
	to_display += "\r\n";
	send(user->getFd(), to_display.c_str(), to_display.length(), 0);
}
