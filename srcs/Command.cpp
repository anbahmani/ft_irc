# include "../include/Server.hpp"

// Constructors

	Command::Command(void) : _name(""), _parameters(std::vector<std::string>(0)){
		return ;
	}

	Command::Command(std::string src) {
		size_t pos;
		int	first = 0;
		while ((pos = src.find(' ')) != std::string::npos)
		{
			if (!first)
			{
				_name = src.substr(0, pos);
				first++;
			}
			else
				_parameters.push_back(src.substr(0, pos));
			// std::cout<<"ajout => "<<src.substr(0, pos)<<'\n';
			src.erase(0, pos + 1);
		}
		if (src.length())
			_parameters.push_back(src);
		
		cmds.insert(std::pair<std::string, int>("PING", PING));
		cmds.insert(std::pair<std::string, int>("PONG", PONG));
		cmds.insert(std::pair<std::string, int>("CAP", CAP));
		cmds.insert(std::pair<std::string, int>("PASS", PASS));
		cmds.insert(std::pair<std::string, int>("NICK", NICK));
		cmds.insert(std::pair<std::string, int>("QUIT", QUIT));
		cmds.insert(std::pair<std::string, int>("PART", PART));
		cmds.insert(std::pair<std::string, int>("JOIN", JOIN));
		cmds.insert(std::pair<std::string, int>("USER", USER));
		cmds.insert(std::pair<std::string, int>("WHOIS", WHOIS));
		cmds.insert(std::pair<std::string, int>("MODE", MODE));
		cmds.insert(std::pair<std::string, int>("OPER", OPER));
		cmds.insert(std::pair<std::string, int>("PRIVMSG", PRIVMSG));
		cmds.insert(std::pair<std::string, int>("NOTICE", NOTICE));
		cmds.insert(std::pair<std::string, int>("INVITE", INVITE));
		cmds.insert(std::pair<std::string, int>("kill", KILL));

		return ;
	}
	
	//Destructor

	Command::~Command(void){
		return ;
	}

	// Getters

	std::string Command::getName(void){
		return this->_name;
	}

	std::vector<std::string> Command::getParameters(void){
		return this->_parameters;
	}

	int	Command::getNbParameters(void)
	{
		return this->_parameters.size();
	}

	std::string Command::getMsg(void){
		std::string r("");
		for (std::vector<std::string>::iterator it = this->_parameters.begin() + 1; it < this->_parameters.end(); it++)
		{
			r.append(*it);
			if (it < this->_parameters.end() - 1)
				r.append(" ");
		}
		return r;
	}

	std::string Command::getFName(void){
		std::string r("");
		for (std::vector<std::string>::iterator it = this->_parameters.begin() + 3; it < this->_parameters.end(); it++)
		{
			r.append(*it);
			if (it < this->_parameters.end() - 1)
				r.append(" ");
		}
		return r;
	}
	// // Setters

	// void Command::setName(std::string name){
	// 	this->_name = name;
	// 	return ;
	// }

	// void Command::setParamters(std::vector<std::string> parameters){
	// 	this->paramters =  parameters;
	// 	return ;
	// }

	// // Parameters methods

	// void addParamter(std::string parameter) {
	// 	this->parameters.push_back(parameter);
	// }