/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brhajji- <brhajji-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 08:27:09 by brhajji-          #+#    #+#             */
/*   Updated: 2023/01/19 17:24:12 by brhajji-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/User.hpp"

User::User(int fd) : fd(fd)
{
}

User::~User()
{
}

//Getters

std::string User::getUsername(void){
	return this->username;
}

std::string User::getNickname(void){
	return this->nickname;
}

int	User::getFd(void)
{
	return this->fd;
}
// e_event User::getEventHooked(void) {
// 	return this->event_hooked;
// }

// Setters

void User::setUsername(std::string username){
	this->username = username;
	return ;
}

void User::setNickname(std::string nickname){
	this->nickname = nickname;
	return ;
}

// Specifics methods

// void User::incrEventHooked(void){
// 	this->event_hooked = static_cast<e_event>(this->event_hooked + 1);
// }
