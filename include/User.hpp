/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahmani <abahmani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 08:27:12 by brhajji-          #+#    #+#             */
/*   Updated: 2023/02/10 05:42:29 by abahmani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include "User.hpp"
#include "irc.hpp"

class User
{
	private:
		int					fd;
		std::string		username;
		std::string		nickname;
		bool			_op;
		bool			_i;
		bool			_w;

		std::string		fullname;
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
		bool 		get_i(void) const;
		bool 		get_w(void) const;
		std::string	getMode(void) const;
	
		std::string getFullname(void);

		void setUsername(std::string);
		void setFullname(std::string);
		void setNickname(std::string nickname);
		void setMode(std::string mode, bool state);
		// Specific methods
		void writeMessage(std::string message);
};

#endif