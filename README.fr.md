[![en](https://img.shields.io/badge/language-en-red.svg)](https://github.com/anbahmani/ft_irc/blob/main/README.md)

# Projet Serveur IRC


## Introduction
Dans le cadre de mon cursus en informatique à l'école 42, j'ai développé un serveur IRC (Internet Relay Chat) en respectant les normes du C++98. Ce projet avait pour but d'approfondir ma compréhension des réseaux et des protocoles de communication, en particulier dans le domaine des systèmes de messagerie.

## Contraintes du Projet
- Langage de Développement : Le serveur est entièrement écrit en C++98, garantissant compatibilité et performance.
- Côté Serveur Uniquement : L'accent a été mis sur le développement du serveur, sans application client accompagnatrice. Les tests ont été effectués avec deux clients IRC : Irssi et NetCat.
- Communication Mono-Serveur : Le serveur ne gère pas la communication de serveur à serveur, se concentrant uniquement sur les connexions et interactions des clients.

## Utilisation
Le serveur est lancé en utilisant la commande suivante :

```
./ircserv <port> <password>
```
- port : Le numéro de port désigné sur lequel le serveur acceptera les connexions entrantes.
- password : Un mot de passe d'authentification requis que les clients IRC doivent utiliser pour se connecter au serveur.

## Fonctionnalités
- Opérations Non-Bloquantes : Le serveur peut gérer plusieurs clients simultanément sans blocage. Il n'utilise pas de bifurcation ; toutes les opérations d'entrée/sortie sont non bloquantes, réalisées grâce à l'utilisation de epoll().
- Compatibilité avec les Clients IRC Standards : Bien que le serveur ne soit pas livré avec son propre client, il est conçu pour être compatible avec divers clients IRC standards.
- Fonctionnalités IRC de Base : Le serveur prend en charge les fonctionnalités essentielles d'IRC, y compris :
	- Authentification
	- Définition d'un pseudonyme et d'un nom d'utilisateur
	- Rejoindre des canaux
	- Envoyer et recevoir des messages privés
	- Fonctionnalités de base pour les utilisateurs et les opérateurs
- Opérations sur les Canaux : Les opérateurs peuvent exécuter des commandes spécifiques affectant la gestion des canaux, telles que :
	- KICK : Éjecter un client du canal
	- INVITE : Inviter un client au canal
	- TOPIC : Changer ou afficher le sujet actuel du canal
	- MODE : Modifier les modes de canal, y compris la définition du canal sur invitation uniquement, la restriction des commandes de sujet aux opérateurs, la définition d'une clé de canal (mot de passe), l'octroi ou la révocation des privilèges d'opérateur, et la définition d'une limite d'utilisateurs pour le canal.

## Conclusion
Ce projet représente une incursion approfondie dans le développement d'applications basées sur le réseau, mettant en valeur les capacités du langage de programmation C++ et les fonctionnalités complexes au sein des systèmes de chat. Il sert de témoignage de compétence pratique dans la création d'un élément fondamental d'une plateforme de communication.