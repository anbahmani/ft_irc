/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahmani <abahmani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 08:27:09 by brhajji-          #+#    #+#             */
/*   Updated: 2023/02/11 03:34:32 by abahmani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/User.hpp"

User::User(int fd) : fd(fd), _op(false), _i(true), _w(false) , _op_chan(false), lastPong(std::time(0)), lastPing(std::time(0)), state(0)
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

int User::getstate(void){
	return this->state;
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

time_t	User::getPong(void)
{
	return this->lastPong;
}

time_t	User::getPing(void)
{
	return this->lastPong;
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

bool	User::get_op_chan(void) const
{
	return (this->_op_chan);
}

bool	User::get_v_chan(void) const
{
	return (this->_op_chan);
}
std::string User::getComment(void) const {
	return (this->comment);
}


// Setters

void User::setstate(int state)
{
	this->state = state;
}

void User::setUsername(std::string username){
	this->username = username;
	return ;
}

void User::setPong(time_t time){
	this->lastPong = time;
	return ;
}

void User::setPing(time_t time){
	this->lastPing = time;
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

void	User::setModeChan(std::string mode, bool state)
{
	if (mode.find('o') != std::string::npos)
		this->_op_chan = state;
	else if (mode.find('v') != std::string::npos)
		this->_v_chan = state;
}

void User::setChannel(std::string channel)
{
	this->_channel = channel;
}

void User::setComment(std::string comment){
	this->comment = comment;
}

// Specifics methods

void User::writeMessage(std::string message){
	message += "\r\n";
	write(this->fd, message.c_str(), message.length());
}
