#ifndef PROTOTYPES_WOODY_WOODPACKER_H
# define PROTOTYPES_WOODY_WOODPACKER_H

int8_t		access_file(t_packer *packer, char *file);
int8_t		check_elf_header(t_packer *packer);
void		chirurgy(t_packer *packer,
			size_t offset,
			size_t size,
			Elf64_Phdr *phdr,
			Elf64_Shdr *shdr);
void		chirurgy(t_packer *packer,
			size_t offset,
			size_t size,
			Elf64_Phdr *phdr,
			Elf64_Shdr *shdr);
uint8_t		crypt_condition(void *data, Elf64_Sym *sym);
void		*elf_access_section(void *data, char *section_name, int umpteenth);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
int8_t		get_payload_zones(t_packer *packer);
int8_t		get_zones_to_crypt(t_packer *packer);
uint8_t		insert_jump(t_packer *packer, int offset_from, int offset_to);
uint8_t		insert_write(t_packer *packer, int offset);
int8_t		is_secure_access(uint64_t size, uint64_t offset, uint64_t to_read);
int			main(int ac, char **av);
void		new_try(Elf64_Ehdr *elf, void *data, t_packer *packer);
int8_t		pack_file(t_packer *packer);
int8_t		print_error(char *self_path, char *error);
void		print_symbol_code(void *data, size_t offset, size_t size);
int8_t		print_usage(char *self_path);
void		test_cypher(void);
void		unit_test_cypher(char *str, int len, char key);

#endif