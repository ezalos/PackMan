#ifndef PROTOTYPES_WOODYWOODPACKER_H
# define PROTOTYPES_WOODYWOODPACKER_H

int8_t	access_file(t_packer *packer, char *file);
void	*elf_access_section(void *data, char *section_name, int umpteenth);
int		main(int ac, char **av);
void	new_try(Elf64_Ehdr *elf, void *data, t_packer *packer);
void	print_symbol_code(void *data, size_t offset, size_t size);
void	test_cypher(void);
void	unit_test_cypher(char *str, int len, char key);

#endif