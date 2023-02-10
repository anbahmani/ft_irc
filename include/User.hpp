/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vahemere <vahemere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 08:27:12 by brhajji-          #+#    #+#             */
/*   Updated: 2023/02/10 11:38:15 by vahemere         ###   ########.fr       */
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

	public:
		// Constructors
		User(int fd);

		// Destructor
		~User();

		//Getters
		int			getFd(void);
		std::string	getUsername(void);
		std::string	getNickname(void);
		bool		getIRCOp(void);
		bool 		get_i(void) const;
		bool 		get_w(void) const;
		std::string	getMode(void) const;
		std::string	getChannel(void) const;
		std::string getFullname(void);
		

		void setUsername(std::string);
		void setFullname(std::string);
		void setNickname(std::string nickname);
		void setMode(std::string mode, bool state);
		void setChannel(std::string channel);
		// Specific methods
		void	writeMessage(std::string message);
		void	addChannel(const std::string& channel);
};

#endif