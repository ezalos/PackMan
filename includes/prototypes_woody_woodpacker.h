#ifndef PROTOTYPES_WOODY_WOODPACKER_H
# define PROTOTYPES_WOODY_WOODPACKER_H

int8_t		access_file(t_packer *packer, char *file);
int8_t		check_elf_header(t_packer *packer);
uint8_t		crypt_condition(void *data, Elf64_Sym *sym);
void		*elf_access_section(void *data, char *section_name, int umpteenth);
int8_t		is_secure_access(uint64_t size, uint64_t offset, uint64_t to_read);
int			main(int ac, char **av);
void		new_try(Elf64_Ehdr *elf, void *data, t_packer *packer);
int8_t		pack_file(t_packer *packer);
void		print_symbol_code(void *data, size_t offset, size_t size);
int8_t		print_usage();
void		test_cypher(void);
void		unit_test_cypher(char *str, int len, char key);

#endif