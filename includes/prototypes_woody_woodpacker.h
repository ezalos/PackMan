#ifndef PROTOTYPES_WOODY_WOODPACKER_H
# define PROTOTYPES_WOODY_WOODPACKER_H

int8_t		access_file(t_packer *packer, char *file);
void		cave_gathering(t_packer *packer);
int8_t		check_elf_header(t_packer *packer);
void		chirurgy(t_packer *packer,
			size_t offset,
			size_t size,
			Elf64_Phdr *phdr,
			Elf64_Shdr *shdr);
t_rbt		*construct_rbt_phdr(t_packer *packer);
void		construct_rbt_shdr(t_packer *packer);
int8_t		create_rbt_phdr(t_rbt **node, t_pheader **hdr, Elf64_Phdr *phdr);
int8_t		create_rbt_shdr(t_rbt **node,
			t_sheader **hdr,
			Elf64_Shdr *shdr,
			t_pheader *parent);
uint8_t		crypt_condition(void *data, Elf64_Sym *sym);
void		*elf_access_section(void *data, char *section_name, int umpteenth);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
int8_t		get_payload_zones(t_packer *packer);
int 		get_program_header_index(t_packer *packer, Elf64_Phdr *phdr);
t_pheader	*get_rbt_phdr_from_shdr(t_rbt *root, Elf64_Shdr *shdr);
int			get_section_header_index(t_packer *packer, Elf64_Shdr *shdr);
int8_t		get_zones_to_crypt(t_packer *packer);
uint8_t		insert_jump(t_packer * packer, int offset_from, int offset_to);
uint8_t		insert_write(t_packer *packer, int offset);
int8_t		is_secure_access(uint64_t mem_size,
			uint64_t offset,
			uint64_t access_size);
// int			main(int ac, char **av);
void		make_array_of_arrays(t_packer *packer);
void		new_try(Elf64_Ehdr *elf, void *data, t_packer *packer);
int8_t		pack_file(t_packer *packer);
void		parse_elf(t_packer *packer);
void 		parse_print(t_packer *packer);
void		phdr_print_tree(t_packer *packer, t_rbt *root);
int8_t		print_error(char *self_path, char *error);
void		print_symbol_code(void *data, size_t offset, size_t size);
int8_t		print_usage(char *self_path);
void		rbt_free_content(void **content);
void		rbt_keep_content(void **content);
long long	t_rbt_compare_phdr(void *an, void *bn);
long long	t_rbt_compare_shdr(void *a, void *b);
void		test_cypher(void);
void		unit_test_cypher(char *str, int len, char key);

#endif