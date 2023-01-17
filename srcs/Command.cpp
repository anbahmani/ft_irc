# include "Command.hpp"

// Constructors

	Command::Command(void) : _name(""), _parameters(std::vector<std:string>(0)){
		return ;
	}

	Command::Command(std::string name, std::vector<std::string> parameters) : _name(name), _parameters(vector<std::string>(parameters)){
		return ;
	}
	
	//Destructor

	Command::~Command(void){
		return ;
	}

	// // Getters

	// std::string Command::getName(void){
	// 	return this->_name;
	// }

	// std::vector<std::string> Command::getParameters(void){
	// 	return this->_parameters;
	// }

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