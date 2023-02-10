/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahmani <abahmani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 06:54:50 by abahmani          #+#    #+#             */
/*   Updated: 2023/02/10 07:49:09 by abahmani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"
# include "User.hpp"
# include "Command.hpp"

class Server;

class Channel
{
	public:
	
    Channel(Server* serv, const std::string& name);
    ~Channel();

    const std::string&              getName() const;
    const std::string&              getTopic() const;
    const std::string&              getKey() const;
    bool                            getMode(char) const;
    const std::string               getModes() const;
    const std::vector<User *>&    	getUsers() const;
    int                             getNbUsers() const;
    int                             getLimit() const;

    void                            setTopic(const std::string&);
    void                            editTopic(User*, const std::string&);
    int                             setMode(char, const std::string& param = std::string());
    int                             unsetMode(char, const std::string& param = std::string());
    void                            changeModes(User *, std::vector<std::string>);
    
    void                            addUser(User *, const std::string& key = std::string());
    int                             addOperator(User *);
    int                             addBan(User *);
    void                            kickUser(User *, User *, const std::string&);
    void                            removeUser(User *, const std::string&);
    void                            removeInvite(User *);
    int                             removeOperator(User *);
    int                             removeBan(User *);

    void                            sendToUsers(const std::string&) const;
    void                            sendTopic(User *) const;
    void                            printUsers(User *) const;
    void                            printInfos(User *) const;
    void                            printBanList(User *) const;
    int                             isUser(User *) const;
    bool                            isOperator(User *) const;
    bool                            isInvited(User *) const;
    bool                            isBanned(User *) const;

    void                            invite(User*, const std::string&);
    void                            printWho(User *) const;
    
    private:

    std::string                     name;
    std::string                     topic;
    std::string                     key;
    std::string                     topic_editor;
    int                             limit;

    std::vector<User *>           users;
    std::vector<User *>           operators;
    std::vector<User *>           invites;
    std::vector<User *>           banned;
    Server*                        serv;
	std::map<const char, bool>		modes;
    
};

#endif