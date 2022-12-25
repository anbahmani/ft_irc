NAME		=	ircserv

CC			=	c++

SRC_DIR		=	$(shell find srcs -type d)
INC_DIR		=	$(shell find includes -type d)
OBJ_DIR		=	obj

vpath %.cpp $(foreach dir, $(SRC_DIR), $(dir):)

SRC			= 	$(foreach dir, $(SRC_DIR), $(foreach file, $(wildcard $(dir)/*.cpp), $(notdir $(file))))

OBJ			=	$(addprefix $(OBJ_DIR)/, $(SRC:%.cpp=%.o))

DEPS		=	$(OBJ:%.o=%.d)

# Compilation flags

CFLAGS		=	-Wall -Wextra -Werror -std=c++98 -g3

IFLAGS		=	$(foreach dir, $(INC_DIR), -I $(dir))

#-------------------------------------------------------------------------------

all:	$(NAME)

show:
	@echo "SRC :\n$(SRC)"
	@echo "OBJ :\n$(OBJ)"
	@echo "CFLAGS :\n$(CFLAGS)"
	@echo "IFLAGS :\n$(IFLAGS)"
	@echo "\n-----\n"
	@echo "Compiling : \n$(CC) $(CFLAGS) $(OBJ) -o $(NAME) "

$(NAME): $(OBJ)
	@echo "-----\nCreating Binary File $@ ... \c"
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "DONE\n-----"

$(OBJ_DIR)/%.o : %.cpp
	@echo "Compiling $@ ... \c"
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(IFLAGS) -MMD -o $@ -c $<
	@echo "DONE"

re:	fclean all

clean:
	@echo "Deleting Objects Directory $(OBJ_DIR) ... \c"
	@$(foreach file, $(OBJ), rm -rf $(file))
	@echo "DONE\n-----"

fclean:	clean
	@echo "Deleting Binary File $(NAME) ... \c"
	@rm -f $(NAME)
	@echo "DONE\n-----"

.PHONY: all show re clean flcean

-include $(DEPS)%