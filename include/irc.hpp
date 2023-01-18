/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brhajji- <brhajji-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 13:53:49 by brhajji-          #+#    #+#             */
/*   Updated: 2023/01/18 06:22:24 by brhajji-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include <iostream>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <unistd.h>
# include <vector>
# include <map>
# include <poll.h>
# include <sys/epoll.h>
# include "User.hpp"
# include "Server.hpp"

#include "Server.hpp"
class Server;
int	pass(char *buf, Server const &server, int client);

enum e_event { 
	CAP_HOOKED,
	PASS_HOOKED,
	NICK_HOOKED,
	USER_HOOKED
	};

#endif