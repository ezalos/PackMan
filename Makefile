# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/20 16:46:57 by ezalos            #+#    #+#              #
#    Updated: 2021/05/10 22:12:33 by rkirszba         ###   ########.fr        #
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
TESTS_DIR	= binaries/
TESTS_CORRUPTED_DIR	= $(TESTS_DIR)corrupted/
TESTS_CORRUPTED_FILES	= $(wildcard $(TESTS_CORRUPTED_DIR)*.out)
TESTS_FILES	= $(wildcard $(TESTS_DIR)*.out)

$(shell mkdir -p $(OBJS_DIR))
SRCS		= $(wildcard $(SRCS_DIR)*.c)
SRCS_ASM	= $(wildcard $(SRCS_DIR)*$(ASM_EXT))
OBJS		= $(SRCS:$(SRCS_DIR)%.c=$(OBJS_DIR)%.o)
OBJS_ASM	= $(SRCS_ASM:$(SRCS_DIR)%$(ASM_EXT)=$(OBJS_DIR)%.o)

RED     	= \e[31m
GREEN   	= \e[32m
YELLOW  	= \e[33m
BLUE		= \e[34m
MAGENTA		= \e[35m
CYAN		= \e[36m
END     	= \e[0m

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
	$(MAKE) -C $(LIB_RBT_DIR)

$(LIB_FT): FORCE
	$(MAKE) -C $(LIB_FT_DIR)

clean:
	$(MAKE) clean -C $(LIB_RBT_DIR)
	$(MAKE) clean -C $(LIB_FT_DIR)
	rm -rf $(OBJS)
	rm -rf $(OBJS_DIR)

fclean: clean
	$(MAKE) fclean -C $(LIB_RBT_DIR)
	$(MAKE) fclean -C $(LIB_FT_DIR)
	rm -rf $(NAME) $(TESTOR)

re : fclean
	$(MAKE) all

##########################
##						##
##		  PERSO			##
##						##
##########################

SUPPORTED_COMMANDS := run tests recur rtest
SUPPORTS_MAKE_ARGS := $(findstring $(firstword $(MAKECMDGOALS)), $(SUPPORTED_COMMANDS))
ifneq "$(SUPPORTS_MAKE_ARGS)" ""
  COMMAND_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(COMMAND_ARGS):;@:)
endif

run: $(NAME)
	rm -f $(PACKED)
	./$(NAME) $(COMMAND_ARGS)
	./$(PACKED)

recur: $(NAME)
	sh tests/recursive_testor.sh $(COMMAND_ARGS)

$(TESTS_CORRUPTED_DIR)%.out: Makefile $(LIB_RBT) $(LIB_FT) $(HEAD_FILES) $(OBJS) $(OBJS_ASM)
	@echo "$(BLUE)Starting test $(YELLOW)$@$(END):"
	@rm -f $(PACKED)
	@(./$(NAME) $@ && ./$(PACKED) && echo "$(GREEN)Test $@ done!$(END)") || echo "$(RED)Test $@ done!$(END)"

$(TESTS_DIR)%.out: Makefile $(LIB_RBT) $(LIB_FT) $(HEAD_FILES) $(OBJS) $(OBJS_ASM)
	@echo "$(BLUE)Starting test $(YELLOW)$@$(END):"
	@rm -f $(PACKED)
	@(sh tests/recursive_testor.sh $@ 100 && echo "$(GREEN)Test $@ done!$(END)") || echo "$(RED)Test $@ done!$(END)"


tests: all $(TESTS_CORRUPTED_FILES) Makefile $(LIB_RBT) $(LIB_FT) $(HEAD_FILES) $(OBJS) $(OBJS_ASM)

rtests: all $(TESTS_FILES) Makefile $(LIB_RBT) $(LIB_FT) $(HEAD_FILES) $(OBJS) $(OBJS_ASM)

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
