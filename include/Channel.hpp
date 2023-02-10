/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vahemere <vahemere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 06:54:50 by abahmani          #+#    #+#             */
/*   Updated: 2023/02/10 13:47:52 by vahemere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# define LIMIT_USER 4

# include "irc.hpp"
# include "User.hpp"
# include "Command.hpp"

class Server;
class User;

class Channel
{
	public:
	
	Channel(Server* serv, const std::string& name);
	~Channel();

	std::vector<User *> getUser(void) const;
	std::string			getModeChan(void) const;

	void				setModeChan(std::string mode, bool state);
	// void				add_user(void);

	void    addUser(User *user); 
	
	private:

	Server                		*_serv;
	std::string           		name;
	std::vector<User *>   		users;
	// std::vector<User *>		operators;
	// std::vector<User *>      can_speak;
	// std::vector<User *>      limit_user;
	bool						_l; //limit user
	bool						_m; // canal modere
	bool						_v; // enleve ou donne la possibilite de parler dans un canal moderer
	bool						_o; // operator
	
};

#endif