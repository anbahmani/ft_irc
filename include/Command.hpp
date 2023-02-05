#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "irc.hpp"

class Command {

	private :

		std::string _name; // name of the commande
		std::vector<std::string> _parameters; // all parameters of the command

	public :

		std::map<std::string, int> cmds;
		// Constructors

		Command(void);
		Command(std::string src);
		
		//Destructor

		~Command(void);

		// Getters

		std::string getName(void);
		std::vector<std::string> getParameters(void);

		// Setters

		void setName(std::string name);
		void setParamters(std::vector<std::string>);

		// Parameters methods

		void addParamter(std::string parameter);


};

#endif