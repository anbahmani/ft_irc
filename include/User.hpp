/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahmani <abahmani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 08:27:12 by brhajji-          #+#    #+#             */
/*   Updated: 2023/02/10 21:07:14 by abahmani         ###   ########.fr       */
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
		bool			_i;
		bool			_w;
		time_t			lastPong;
		time_t			lastPing;
		bool			dead;
		std::string						fullname;
		std::string						_channel;
		std::string						comment;
		std::vector<std::string>		channels;

	public:
		// Constructors
		User(int fd);

		// Destructor
		~User();

		//Getters
		int			getFd(void);
		std::string getUsername(void);
		std::string getNickname(void);
		time_t		getPong();
		time_t		getPing();
		bool		getDead();
		bool		getIRCOp(void);
		bool 		get_i(void) const;
		bool 		get_w(void) const;
		std::string	getMode(void) const;
		std::string	getChannel(void) const;
		std::string getFullname(void);
		std::string getComment(void) const;
		

		void setUsername(std::string);
		void setPong(time_t);
		void setPing(time_t);
		void setDead(bool);
		void setFullname(std::string);
		void setNickname(std::string nickname);
		void setMode(std::string mode, bool state);
		void setChannel(std::string channel);
		void setComment(std::string comment);
		// Specific methods
		void	writeMessage(std::string message);
		void	addChannel(const std::string& channel);
};

#endif