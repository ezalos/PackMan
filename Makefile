# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/20 16:46:57 by ezalos            #+#    #+#              #
#    Updated: 2021/03/25 15:50:42 by ezalos           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= woody_woodpacker
PACKED		= woody.out
# TESTOR		= lib_testor.out

CC			= gcc
AR			= ar -rcs
NS			= nasm -f elf64

CFLAGS		= -Wall -Werror -Wextra -g3

#For developping purposes:
# CFLAGS 		+= -fsanitize=address,undefined -g3
# CFLAGS 		+= -g

LIB_RBT_DIR		= librbt/
LIB_RBT			= $(LIB_RBT_DIR)librbt.a
LIB_RBT_INC		= $(LIB_RBT_DIR)includes/
LIB_RBT_HEAD	= $(LIB_RBT_INC)librbt.h

LIB_FT_DIR 		= libft/
LIB_FT			= $(LIB_FT_DIR)libft.a
LIB_FT_INC		= $(LIB_FT_DIR)includes/
LIB_FT_HEAD		= $(LIB_FT_INC)libft.h

ASM_EXT		= .asm

SRCS_DIR	= srcs/
HEAD_DIR	= includes/
HEAD_FILES	= $(wildcard $(HEAD_DIR)*.h)
OBJS_DIR	= objs/

$(shell mkdir -p $(OBJS_DIR))
SRCS		= $(wildcard $(SRCS_DIR)*.c)
SRCS_ASM	= $(wildcard $(SRCS_DIR)*$(ASM_EXT))
OBJS		= $(SRCS:$(SRCS_DIR)%.c=$(OBJS_DIR)%.o)
OBJS_ASM	= $(SRCS_ASM:$(SRCS_DIR)%$(ASM_EXT)=$(OBJS_DIR)%.o)


##########################
##						##
##		  BASIC			##
##						##
##########################

all: $(NAME)

$(NAME): $(OBJS) $(OBJS_ASM) $(LIB_RBT) $(LIB_FT) $(HEAD_FILES)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(OBJS_ASM) -I $(HEAD_DIR) -I $(LIB_RBT_INC) $(LIB_RBT) -I $(LIB_FT_INC) $(LIB_FT)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c Makefile $(LIB_RBT) $(LIB_FT) $(HEAD_FILES)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(HEAD_DIR) -I$(LIB_RBT_INC) -L$(LIB_RBT_DIR) -I$(LIB_FT_INC) -L$(LIB_FT_DIR)

$(OBJS_DIR)%.o: $(SRCS_DIR)%$(ASM_EXT) Makefile $(LIB_RBT) $(LIB_FT)
	$(NS) $< -o $@ -I$(HEAD_DIR) -I$(LIB_RBT_INC)  -I$(LIB_FT_INC)  
#-L$(LIB_DIR)

$(LIB_RBT): FORCE
	make -C $(LIB_RBT_DIR)

$(LIB_FT): FORCE
	make -C $(LIB_FT_DIR)

clean:
	rm -rf $(OBJS)
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME) $(TESTOR)

re : fclean
	$(MAKE) all

##########################
##						##
##		  PERSO			##
##						##
##########################

SUPPORTED_COMMANDS := run tests
SUPPORTS_MAKE_ARGS := $(findstring $(firstword $(MAKECMDGOALS)), $(SUPPORTED_COMMANDS))
ifneq "$(SUPPORTS_MAKE_ARGS)" ""
  COMMAND_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(COMMAND_ARGS):;@:)
endif

run: $(NAME)
	rm -f $(PACKED)
	./$(NAME) $(COMMAND_ARGS)
	chmod 777 $(PACKED)
	chmod +x $(PACKED)

prototypes:
	python3 .tmp/prototype_catcher.py srcs includes/prototypes_$(NAME).h $(NAME)

REQUEST 		= 'read -p "Enter a commit message:	" pwd && echo $$pwd'
COMMIT_MESSAGE ?= $(shell bash -c $(REQUEST))
git :
		git add -A
		git status
		(echo -n "Are you sure? [y/N] " && read ans && [ $${ans:-N} = y ])
		git commit -m "$(COMMIT_MESSAGE)"
		git push

.PHONY: clean fclean

FORCE:
