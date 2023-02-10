/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brhajji- <brhajji-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 08:27:12 by brhajji-          #+#    #+#             */
/*   Updated: 2023/02/10 23:39:45 by brhajji-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include "User.hpp"
#include "irc.hpp"

class User
{
	private:
		int								fd;
		std::string						username;
		std::string						nickname;
		std::string						fullname;
		std::string						_channel;
		bool							_op;
		bool							_i;
		bool							_w;
		std::vector<std::string>		channels;
		time_t							lastPong;
		time_t							lastPing;
		int								state;

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
		int			getstate();
		bool 		getIRCOp(void);
		bool 		get_i(void) const;
		bool 		get_w(void) const;
		std::string	getMode(void) const;
		std::string	getChannel(void) const;
		std::string getFullname(void);
		

		void setUsername(std::string);
		void setPong(time_t);
		void setPing(time_t);
		void setstate(int);
		void setFullname(std::string);
		void setNickname(std::string nickname);
		void setMode(std::string mode, bool state);
		void setChannel(std::string channel);
		// Specific methods
		void	writeMessage(std::string message);
		void	addChannel(const std::string& channel);
};

#endif