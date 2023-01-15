/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahmani <abahmani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 13:53:49 by brhajji-          #+#    #+#             */
/*   Updated: 2023/01/15 18:25:53 by abahmani         ###   ########.fr       */
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
# include <poll.h>
# include <sys/epoll.h>
# include "signal.h"

#include <cstdio> // test


enum e_event { 
	CAP_HOOKED,
	PASS_HOOKED,
	NICK_HOOKED,
	USER_HOOKED
	};

#endif