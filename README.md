[![fr](https://img.shields.io/badge/language-fr-blue.svg)](https://github.com/anbahmani/ft_irc/blob/main/README.fr.md)

# IRC Server Project

## Introduction
As a part of the computer science program at 42 school, I have developed an IRC (Internet Relay Chat) server from scratch, adhering to the standards of C++98. This project was designed to deepen my understanding of networking and communication protocols, specifically within the realm of chat systems.

## Project Constraints
- Development Language: The entire server is written in C++98, ensuring compatibility and performance efficiency.
- Server-Side Only: The focus was on server development, without an accompanying client application. Testing was conducted with two IRC clients : Irssi and NetCat.
- Single-Server Communication: The server does not handle server-to-server communication, focusing solely on client connections and interactions.

## Usage
The server is initiated using the following command:

```
./ircserv <port> <password>
```

- port: The designated port number on which the server will accept incoming connections.
- password: A required authentication password that IRC clients must use to connect to the server.

## Key Features
- Non-Blocking Operations: The server can manage multiple clients simultaneously without blocking. It does not use forking; all I/O operations are non-blocking, achieved through the use of epoll().
- Compatibility with Standard IRC Clients: While the server does not come with its proprietary client, it is designed to be compatible with various standard IRC clients.
- Core IRC Features: The server supports essential IRC functionalities, including:
	- Authentication
	- Nickname and username setting
	- Joining channels
	- Sending and receiving private messages
	- Basic user and operator functionalities
- Channel Operations: Operators can execute specific commands affecting channel management, such as:
	- KICK: Ejecting a client from the channel
	- INVITE: Inviting a client to the channel
	- TOPIC: Changing or displaying the current channel topic
	- MODE: Altering channel modes, including setting the channel to invitation-only, restricting topic commands to operators, setting a channel key (password), granting or revoking operator privileges, and setting a user limit for the channel.

## Conclusion
This project represents a comprehensive foray into network-based application development, emphasizing the C++ programming language's capabilities and the complex functionalities within chat systems. It serves as a testament to practical skill in creating a foundational piece of a communication platform.