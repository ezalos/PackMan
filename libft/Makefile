# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rkirszba <rkirszba@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/01/12 14:42:41 by rkirszba          #+#    #+#              #
#    Updated: 2021/01/26 20:28:15 by rkirszba         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= libft.a


CC		= gcc

CFLAGS	= -Wall\
		  -Wextra\
		  -Werror

#For developping purposes:
#CFLAGS 	+= -fsanitize=address,undefined\
				   -g3

SRCS_DIR		= srcs/
SRCS_NAMES		= ft_strlen.c\
				  ft_strnlen.c\
				  ft_strcpy.c\
				  ft_strncpy.c\
				  ft_strcat.c\
				  ft_strdup.c\
				  ft_strndup.c\
				  ft_strjoin.c\
				  ft_strchr.c\
				  ft_strnchr.c\
				  ft_strcmp.c\
				  ft_strncmp.c\
				  ft_putstr.c\
				  ft_putstr_fd.c\
				  ft_memcpy.c\
				  ft_bzero.c\
				  ft_memset.c\
				  ft_list_new.c\
				  ft_list_append.c\
				  ft_list_prepend.c\
				  ft_list_free.c\
				  ft_btree_new.c\
				  ft_btree_insert.c\
				  ft_btree_inorder.c\
				  ft_btree_inrorder.c\
				  ft_btree_free.c


INCS_DIR		= includes/
INCS_NAMES		= libft.h

OBJS_DIR		= objs/
$(shell mkdir -p $(OBJS_DIR))


SRCS	= $(addprefix $(SRCS_DIR), $(SRCS_NAMES))
INCS	= $(addprefix $(INCS_DIR), $(INCS_NAMES))
OBJS	= $(SRCS:$(SRCS_DIR)%.c=$(OBJS_DIR)%.o)



all: $(NAME)

$(NAME): $(OBJS)
	ar rc $@ $^
	ranlib $@
	@echo "made libft"

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