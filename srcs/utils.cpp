/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vahemere <vahemere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 16:32:01 by vahemere          #+#    #+#             */
/*   Updated: 2023/02/09 16:32:24 by vahemere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"

int	check_mode(std::string mode)
{
	if ((mode.find("i") != std::string::npos 
		|| mode.find("w") != std::string::npos
		|| mode.find("o") != std::string::npos)
		&& (mode.find("+") != std::string::npos || mode.find("-") != std::string::npos))
		return (1);
	return (0);
}

int check_if_mode(User *user, std::string mode)
{
	if (mode.find("+i") != std::string::npos && user->get_i() == false)
	{
		user->setMode(mode, true);
		return (1);
	}
	if (mode.find("+w") != std::string::npos && user->get_w() == false)
	{
		user->setMode(mode, true);
		return (1);
	}
	return (0);
}

int check_if_not_mode(User *user, std::string mode)
{
	if (mode.find("-i") != std::string::npos && user->get_i() == true)
	{
		user->setMode(mode, false);
		return (1);
	}
	if (mode.find("-w") != std::string::npos && user->get_w() == true)
	{
		user->setMode(mode, false);
		return (1);
	}
	if (mode.find("-o") != std::string::npos && user->getIRCOp() == true)
	{
		user->setMode(mode, false);
		return (1);
	}
	return (0);
}