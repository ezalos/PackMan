# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/02/19 13:06:33 by rkirszba          #+#    #+#              #
#    Updated: 2021/02/19 13:09:56 by rkirszba         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= librbt.a


CC		= gcc

CFLAGS	= -Wall\
		  -Wextra\
		  -Werror

#For developping purposes:
#CFLAGS 	+= -fsanitize=address,undefined\
				   -g3

SRCS_DIR		= srcs/
SRCS_NAMES		= tree_free.c\
				  tree_genealogy.c\
				  tree_get.c\
				  tree_inorder.c\
				  tree_inrorder.c\
				  tree_insert.c\
				  tree_insert_case.c\
				  tree_insert_recurse.c\
				  tree_insert_repair.c\
				  tree_len.c\
				  tree_new_node.c\
				  tree_print.c\
				  tree_print_node.c\
				  tree_recoloring.c\
				  tree_remove.c\
				  tree_remove_delete_case_a.c\
				  tree_remove_delete_case_b.c\
				  tree_remove_utils.c\
				  tree_rotations.c


INCS_DIR		= includes/
INCS_NAMES		= librbt.h

OBJS_DIR		= objs/
$(shell mkdir -p $(OBJS_DIR))


SRCS	= $(addprefix $(SRCS_DIR), $(SRCS_NAMES))
INCS	= $(addprefix $(INCS_DIR), $(INCS_NAMES))
OBJS	= $(SRCS:$(SRCS_DIR)%.c=$(OBJS_DIR)%.o)



all: $(NAME)

$(NAME): $(OBJS)
	ar rc $@ $^
	ranlib $@
	@echo "made librbt"

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c $(INCS) Makefile
	$(CC) $(CFLAGS) -o $@ -c $< -I./$(INCS_DIR)

clean:
	$(shell rm -rf $(OBJS))
	$(shell rm -rf $(OBJS_DIR))

fclean: clean
	$(shell rm -rf $(NAME))

re: fclean
	make all

.PHONY: all clean fclean re