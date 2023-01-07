/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brhajji- <brhajji-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 13:48:15 by brhajji-          #+#    #+#             */
/*   Updated: 2023/01/02 07:48:18 by brhajji-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Server.hpp"

int main(int ac, char **av)
{
	(void)(av);
	if (ac != 3)
		std::cerr<<"Invalid args, exemple : ./ircserv <port> <password>."<<std::endl;
	else
	{
		Server test;
		test.init();
	}
	return (0);
}