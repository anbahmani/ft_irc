#include "Server.hpp"

int user(User &user, Command &command){
	if (user.getEventHooked() != NICK_HOOKED)
		return (1);												// reply error
	if (user.getNbParams() < 4)
		return (2);												// reply error
	if (command.getParameters()[3][0] != ':')
		return (3);												// print error
	if (command.getParameters()[3].length() == 1)
		return (4);												// reply error
	user.setUsername(command.getParameters()[0]);
	std::string realname = "";
	for (unsigned int i = 3; i < command.getParameters().size(); i++){
		realname += command.getParameters()[i];
		if (i < command.getParameters().size() - 1)
			realname += " ";
	}
	user.setRealname(realname);
	user.incrEventHooked();
}