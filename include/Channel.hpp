/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vahemere <vahemere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 06:54:50 by abahmani          #+#    #+#             */
/*   Updated: 2023/02/10 11:44:59 by vahemere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

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
    
    private:

    std::string                   name;
    std::string                   key;
    std::vector<User *>           users;
    std::vector<User *>           operators;
    std::vector<User *>           invites;
    std::vector<User *>           banned;
    
};

#endif