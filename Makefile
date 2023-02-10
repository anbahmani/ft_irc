# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vahemere <vahemere@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/13 02:16:29 by vahemere          #+#    #+#              #
#    Updated: 2023/02/09 19:11:54 by vahemere         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

OBJS_DIR = objs
SRCS_DIR = $(shell find srcs -type d)

vpath %.cpp $(foreach dir, $(SRCS_DIR), $(dir))
SRCS = main.cpp Server.cpp  User.cpp Command.cpp replies.cpp utils.cpp\

OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:%.cpp=%.o))

DEPS = $(OBJS:%.o=%.d)

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g3 #-fsanitize=address

#fonts color
BLACK =      \033[30m
RED =        \033[31m
YELLOW =     \033[33m
GREEN =      \033[32m
BLUE =       \033[34m
PURPLE =     \033[35m
WHITE =      \033[7m
END =        \033[0m

all: $(NAME)

$(NAME) :  header compiling $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(PURPLE)] 100%$(END) \n\n$(YELLOW)-----$(END)\n\n$(PURPLE)./irc$(END) ✔️"

$(OBJS_DIR)/%.o : %.cpp
	@mkdir -p $(OBJS_DIR)
	@$(CC) $(CFLAGS) -MMD -o $@ -c $<
	@printf "$(GREEN)██████$(END)"
	
clean :
	@rm -rf $(OBJS_DIR)
	@echo "$(GREEN)cleaning objs$(END)✔️"

fclean : header clean
	@rm -rf $(NAME)
	@echo "$(GREEN)cleaning ./irc$(END)✔️\n\n"

re : fclean all

header : 
	@echo "\n\n$(YELLOW) (   (           (       (              (   	$(END)" 
	@echo "$(YELLOW) )\ ))\ )  (     )\ )    )\ )           )\ ) 	$(END)" 
	@echo "$(YELLOW)(()/(()/(  )\   (()/((  (()/((   (  (  (()/( 	$(END)" 
	@echo "$(YELLOW) /(_))(_)|((_)   /(_))\  /(_))\  )\ )\  /(_))	$(END)" 
	@echo "$(YELLOW)(_))(_)) )\___  (_))((_)(_))((_)((_|(_)(_))  	$(END)" 
	@echo "$(PURPLE)|_ _| _ ((/ __| / __| __| _ \ \ / /| __| _ \ 		$(END)" 
	@echo "$(PURPLE) | ||   /| (__  \__ \ _||   /\ V / | _||   / 		$(END)" 
	@echo "$(PURPLE)|___|_|_\ \___| |___/___|_|_\ \_/  |___|_|_\ 		$(END)\n\n"

compiling :
	@printf "$(PURPLE)Compiling: [$(END)"
	
.PHONY: all clean fclean re header compiling

-include $(DEPS)