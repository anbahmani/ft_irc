/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vahemere <vahemere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 08:27:09 by brhajji-          #+#    #+#             */
/*   Updated: 2023/02/10 11:38:21 by vahemere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/User.hpp"

User::User(int fd) : fd(fd), _op(false), _i(true), _w(false)
{
}

User::~User(){
	if (this->fd > 0)
		close(this->fd);
}

void	User::addChannel(const std::string& channel) { channels.push_back(channel); }

//Getters

std::string User::getUsername(void){
	return this->username;
}

std::string User::getNickname(void){
	return this->nickname;
}

std::string User::getFullname(void){
	return this->fullname;
}

std::string User::getChannel(void) const{
	return (this->_channel);
}


int	User::getFd(void)
{
	return this->fd;
}

bool User::getIRCOp(void)
{
	return this->_op;
}

bool User::get_i(void) const
{
	return this->_i;
}

bool User::get_w(void) const
{
	return this->_w;
}

std::string	User::getMode(void) const
{
	std::string mode = "";

	if (this->_op == true)
		mode += "o";
	if (this->_i == true)
		mode += "i";
	if (this->_w == true)
		mode += "w";
	return (mode);
}


// Setters

void User::setUsername(std::string username){
	this->username = username;
	return ;
}

void User::setFullname(std::string fullname){
	this->fullname = fullname;
	return ;
}

void User::setNickname(std::string nickname){
	this->nickname = nickname;
	return ;
}

void User::setMode(std::string mode, bool state)
{
	if (!mode.empty())
	{
		if (mode.find("+w") != std::string::npos)
			this->_w = state;
		else if (mode.find("+i") != std::string::npos)
			this->_i = state;
		else if (mode.find("IRCOP") != std::string::npos)
			this->_op = state;
		else if (mode.find("-i") != std::string::npos)
			this->_i = state;
		else if (mode.find("-w") != std::string::npos)
			this->_w = state;
		else if (mode.find("-o") != std::string::npos)
			this->_op = state;
	}
	return ;
}

void User::setChannel(std::string channel)
{
	this->_channel = channel;
}

// Specifics methods

void User::writeMessage(std::string message){
	message += "\r\n";
	write(this->fd, message.c_str(), message.length());
}
