/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahmani <abahmani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 08:27:12 by brhajji-          #+#    #+#             */
/*   Updated: 2023/01/15 18:28:38 by abahmani         ###   ########.fr       */
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
		std::string		username;
		std::string		nickname;
		e_event	event_hooked;

	public:
	
		// Constructors
		User(sockaddr_in address, int fd);

		// Destructor
		~User();

		//Getters
		int	getFd(void);
		std::string getUsername(void);
		std::string getNickname(void);
		e_event 	getEventHooked(void);

		// Setters
		void setUsername(std::string);
		void setNickname(std::string);

		// Specific methods
		void incrEventHooked(void);
};

#endif