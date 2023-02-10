/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brhajji- <brhajji-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 11:59:10 by vahemere          #+#    #+#             */
/*   Updated: 2023/02/10 04:52:52 by brhajji-         ###   ########.fr       */
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
		case RPL_WELCOME:				//001
			rpl = "";
		case RPL_YOURHOST:				//002
			rpl = "";
		case RPL_CREATED:				//003
			rpl = "";
		case RPL_MYINFO	:				//004
			rpl = "";
		case RPL_ISUPPORT:				//005
			rpl = "";
		case RPL_UMODEIS:				//221
			rpl = ": Your user mode is +" + user->getMode();
			break ;
		case RPL_WHOISUSER:				//311
			rpl = "";
		case RPL_ENDOFWHO:				//315
			rpl = "";
		case RPL_ENDOFWHOIS:			//318
			rpl = "";
		case RPL_LISTSTART:				//321
			rpl = "";
		case RPL_LIST	:				//322
			rpl = "";
		case RPL_LISTEND:				//323
			rpl = "";
		case RPL_CHANNELMODEIS:			//324
			rpl = "";
		case RPL_NOTOPIC:				//331
			rpl = "";
		case RPL_TOPIC	:				//332
			rpl = "";
		case RPL_TOPICWHOTIME:			//333
			rpl = "";
		case RPL_INVITING:				//341
			rpl = "";
		case RPL_WHOREPLY:				//352
			rpl = "";
		case RPL_NAMREPLY:				//353
			rpl = "";
		case RPL_ENDOFNAMES:			//366
			rpl = "";
		case RPL_BANLIST:				//367
			rpl = "";
		case RPL_ENDOFBANLIST:			//368
			rpl = "";
		case RPL_YOUREOPER:				//381
			rpl = ": You are now an IRC operator";
			break ;
		default:
			break;
	}
	
	std::cout << errcode << std::endl;
	switch (errcode)
	{
		case ERR_NOSUCHNICK:		//401
			err = cmd->getParameters()[0] + ": No such nick/channel";
			break ;
		case ERR_NOSUCHCHANNEL:    	//403
			err = ":No such channel";
		case ERR_CANNOTSENDTOCHAN:  //404
			err = "";
		case ERR_TOOMANYCHANNELS:   //405
			err = "";
		case ERR_TOOMANYTARGETS:    //407
			err = "";
		case ERR_NORECIPIENT:       //411
			err = "";
		case ERR_NOTEXTTOSEND:      //412
			err = "";
		case ERR_UNKNOWNCOMMAND:    //421
			err = "";
		case ERR_NONICKNAMEGIVEN:   //431
			err = "";
		case ERR_ERRONEUSNICKNAME:  //432
			err = "";
		case ERR_NICKNAMEINUSE:     //433
			err = ":Nickname is already in use";
		case ERR_USERNOTINCHANNEL:  //441
			err = "";
		case ERR_NOTONCHANNEL:      //442
			err = ":You are not on that channel";
		case ERR_USERONCHANNEL:     //443
			err = "";
		case ERR_NEEDMOREPARAMS:	//461
			err = cmd->getName() + ": Not enough parameters";
			break ;
		case ERR_ALREADYREGISTERED: //462
			err = "";
		case ERR_PASSWDMISMATCH:    //464
			err = ": Password incorect";
		case ERR_CHANNELISFULL:     //471
			err = " :Cannot join channel";
		case ERR_INVITEONLYCHAN:    //473
			err = ":Cannot join channel";
		case ERR_BANNEDFROMCHAN:    //474
			err = "";
		case ERR_BADCHANNELKEY:     //475
			err = ":Bad Channel Key ";
		case ERR_NOPRIVILEGES:      //481
			err = "";
		case ERR_CHANOPRIVISNEEDED: //482
			err = "";
		case ERR_UMODEUNKNOWNFLAG:	//501
			err = ": Unknown MODE flag";
			break ;
		case ERR_USERSDONTMATCH:	//502
			err = ": Cant change mode for other users";
			break ;
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
