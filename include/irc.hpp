/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahmani <abahmani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 13:53:49 by brhajji-          #+#    #+#             */
/*   Updated: 2023/02/11 01:20:35 by abahmani         ###   ########.fr       */
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
# include <fcntl.h>
# include <unistd.h>
# include <vector>
# include <algorithm>
# include <map>
# include <poll.h>
# include <sys/epoll.h>
# include <sstream>
# include <csignal>
# include <ctime>
# include "User.hpp"
# include "Server.hpp"
# include "Command.hpp"
# include "Channel.hpp"


class Server;
extern bool g_signal;
// replies.cpp
void	reply(int rplcode, int rplerror, User *user, Command *cmd, Server &server, Channel *chan);
void	display(std::string to_display, User *user);

// utils.cpp
int		check_mode(std::string mode);
int		check_if_mode(User *user, std::string mode);
int		check_if_not_mode(User *user, std::string mode);

//Channel modes
#define     KEY         'k'
#define     INVITE      'i'
#define     LIMIT       'l'
#define     MODERATE    'm'
#define     NO_EXTERN   'n'
#define     SECRET      's'
#define     TOPIC       't'
#define     BAN         'b'
#define     CHANOP      'o'
#define		WALLOPS		'w'

#endif