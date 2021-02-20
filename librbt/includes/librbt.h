/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   librbt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezalos <ezalos@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/19 13:12:23 by rkirszba          #+#    #+#             */
/*   Updated: 2021/02/20 19:31:12 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TREE_H
# define TREE_H

# include <limits.h>
# include <stdlib.h>
# include <inttypes.h>
# include <stdio.h>

# define TREE_LEFT 1
# define TREE_RIGHT 2
# define TREE_NONE 0
# define TREE_ERROR -1

enum				e_color {BLACK, RED};

typedef struct		s_rbt
{
	struct s_rbt	*parent;
	struct s_rbt	*left;
	struct s_rbt	*right;
	void			*content;
	enum e_color	color;
}					t_rbt;

typedef	int			(t_rbt_compare)				(void *, void *);
typedef	long long	(t_rbt_compare_long_long)	(void *, void *);
typedef	int			(t_rbt_inorder)				(t_rbt *);
typedef	void		*(t_rbt_inorder_ptr)		(t_rbt *);
typedef	void		(t_rbt_free_content)		(void **);

void				padding_after(t_rbt *node);
void				padding_before(t_rbt *node, size_t space);
void				tree_copy_values(t_rbt *dest, t_rbt *src);
void				tree_cut_leaf(t_rbt *node);
void				tree_delete_case_1(t_rbt *node);
void				tree_delete_case_2(t_rbt *node);
void				tree_delete_case_2a(t_rbt *node);
void				tree_delete_case_2b(t_rbt *node);
void				tree_delete_case_2bi_i(t_rbt *node);
void				tree_delete_case_2bi_ii(t_rbt *node);
void				tree_delete_case_2bii(t_rbt *node);
t_rbt				*tree_delete_node(t_rbt *node);
void				tree_free(t_rbt *root, t_rbt_free_content *func);
t_rbt				*tree_get_in_order_pred(t_rbt *node);
t_rbt				*tree_get_node_th(t_rbt *root, int *umpteenth);
t_rbt				*tree_get_recurse_func(t_rbt *root,
					void *content,
					t_rbt_compare *func);
t_rbt				*tree_get_recurse_func_ll(t_rbt *root, void *content,
					t_rbt_compare_long_long *func);
t_rbt				*tree_grand_parent(t_rbt *node);
int					tree_inorder(t_rbt *root, t_rbt_inorder *func);
void				*tree_inorder_ptr(t_rbt *root, t_rbt_inorder_ptr *func);
int					tree_inrorder(t_rbt *root, t_rbt_inorder *func);
void				*tree_inrorder_ptr(t_rbt *root, t_rbt_inorder_ptr *func);
void				tree_insert_case_1(t_rbt *n);
void				tree_insert_case_2(t_rbt *n);
void				tree_insert_case_3(t_rbt *n);
void				tree_insert_case_4(t_rbt *n);
t_rbt				*tree_insert_func(t_rbt *root, t_rbt *new_node,
					void *content,
					t_rbt_compare *func);
t_rbt				*tree_insert_func_ll(t_rbt *root,
					t_rbt *new_node,
					void *content,
					t_rbt_compare_long_long *func);
void				tree_insert_recurse_func(t_rbt *root,
					t_rbt *n,
					t_rbt_compare *func);
void				tree_insert_recurse_func_ll(t_rbt *root, t_rbt *n,
					t_rbt_compare_long_long *func);
void				tree_insert_repair(t_rbt *n);
int					tree_len(t_rbt *tree);
t_rbt				*tree_new_node(void *content, t_rbt *new_node);
t_rbt				*tree_parent(t_rbt *node);
void				tree_permute_nodes(t_rbt *node1, t_rbt *node2);
void				tree_print(t_rbt *node, size_t deep);
void				tree_print_elem(t_rbt *node);
void				tree_print_node(t_rbt *node);
void				tree_print_node_color(t_rbt *node);
void				tree_recoloring(t_rbt *node);
void				tree_replace_node(t_rbt *node, t_rbt *child);
t_rbt				*tree_root(t_rbt *node);
void				tree_rot_left(t_rbt *node);
void				tree_rot_right(t_rbt *node);
t_rbt				*tree_sibling(t_rbt *node);
void				**tree_to_array(t_rbt *root);
t_rbt				*tree_uncle(t_rbt *node);
int8_t				tree_which_child(t_rbt *node);

#endif