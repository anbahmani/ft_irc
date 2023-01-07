/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brhajji- <brhajji-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 08:27:12 by brhajji-          #+#    #+#             */
/*   Updated: 2023/01/02 08:35:02 by brhajji-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include "irc.hpp"

class User
{
	private:
		struct sockaddr_in	address;
		int					fd;
	public:
		User(sockaddr_in address, int fd);
		~User();
};

User::User(sockaddr_in address, int fd) : address(address), fd(fd)
{
}

User::~User()
{
}


#endif