/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahmani <abahmani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 08:27:12 by brhajji-          #+#    #+#             */
/*   Updated: 2023/02/07 19:15:18 by abahmani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include "User.hpp"
#include "irc.hpp"

class User
{
	private:
		//struct sockaddr_in	address;
		int					fd;
		std::string		username;
		std::string		nickname;
		bool			_op;

		std::string		fullname;
		//e_event			event_hooked;
	public:
		// Constructors
		User(int fd);

		// Destructor
		~User();

		//Getters
		int	getFd(void);
		std::string getUsername(void);
		std::string getNickname(void);
		bool getIRCOp(void);

		std::string getFullname(void);
		//e_event 	getEventHooked(void);

		void setUsername(std::string);
		void setFullname(std::string);
		void setIRCOp(bool state);
		void setNickname(std::string nickname);
		// Specific methods
		void writeMessage(std::string message);
		// void incrEventHooked(void);
};

#endif