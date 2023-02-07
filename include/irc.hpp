/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahmani <abahmani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 13:53:49 by brhajji-          #+#    #+#             */
/*   Updated: 2023/02/07 18:27:07 by abahmani         ###   ########.fr       */
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
# include <algorithm>
# include <map>
# include <poll.h>
# include <sys/epoll.h>
# include <sstream>
# include "User.hpp"
# include "Server.hpp"
# include "Command.hpp"

class Server;

void	reply(int rplcode, int rplerror, User *user, Server &server);
void	display(std::string to_display, User *user);

enum e_event { 
	CAP_HOOKED,
	PASS_HOOKED,
	NICK_HOOKED,
	USER_HOOKED
	};

#endif