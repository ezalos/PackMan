#ifndef PROTOTYPES_WOODY_WOODPACKER_H
# define PROTOTYPES_WOODY_WOODPACKER_H

int8_t		access_file(t_packer *packer, char *file);
ssize_t		bytecode_inject(t_packer *packer,
			t_list *zones,
			t_zone *zone,
			t_btc *inst);
uint8_t		can_i_write(t_zone *zone, t_btc *inst);
void		cave_gathering(t_packer *packer);
void		cave_gathering_phdr(t_packer *packer);
void		change_endian(void *data, int size);
int8_t		check_elf_header(t_packer *packer);
void		chirurgy(t_packer *packer,
			size_t offset,
			size_t size,
			Elf64_Phdr *phdr,
			Elf64_Shdr *shdr);
t_rbt		*construct_rbt_phdr(t_packer *packer);
void		construct_rbt_shdr(t_packer *packer);
t_btc		*create_btc(int type);
int8_t		create_rbt_phdr(t_rbt **node, t_pheader **hdr, Elf64_Phdr *phdr);
int8_t		create_rbt_shdr(t_rbt **node,
			t_sheader **hdr,
			Elf64_Shdr *shdr,
			t_pheader *parent);
uint8_t		crypt_condition(void *data, Elf64_Sym *sym);
void		crypt_zone(uint8_t *zone, size_t len, uint8_t *permutations);
void		crypt_zones(t_packer *packer);
void		data_filler_cave(t_pheader *hdr, t_zone *zone);
void		data_filler_zone_to_crypt(t_pheader *hdr, t_zone *zone);
void		*elf_access_section(void *data, char *section_name, int umpteenth);
t_pheader	*find_t_pheader_from_phdr(t_packer *packer, Elf64_Phdr *a);
void		free_btc(t_btc *btc);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
int8_t		get_payload_zones(t_packer *packer);
Elf64_Phdr	*get_phdr_from_array(t_packer *packer, int i);
int 		get_program_header_index(t_packer *packer, Elf64_Phdr *phdr);
t_pheader	*get_rbt_phdr_from_shdr(t_rbt *root, Elf64_Shdr *shdr);
int			get_section_header_index(t_packer *packer, Elf64_Shdr *shdr);
t_list		*get_zones(t_packer *packer, uint8_t type, uint8_t flags,
			void (*data_filler)(t_pheader*, t_zone*));
int8_t		get_zones_to_crypt(t_packer *packer);
void		init_key(uint8_t *key);
void		init_permutations(uint8_t *permutations);
uint8_t		insert_jump(t_packer *packer, int offset_from, int offset_to);
uint8_t		insert_write(t_packer *packer, int offset);
uint8_t		is_phdr_contained(Elf64_Phdr *a, Elf64_Phdr *b);
uint8_t		is_phdr_overlap(Elf64_Phdr *a, Elf64_Phdr *b);
uint8_t		is_phdr_superposed(Elf64_Phdr *a, Elf64_Phdr *b);
uint8_t		is_same_endian(t_packer *packer);
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
long long	phdr_space_between(Elf64_Phdr *a, Elf64_Phdr *b);
long long	phdr_space_between_ends(Elf64_Phdr *a, Elf64_Phdr *b);
void		print_cave_gathering_legend(void);
void		print_cave_phdr(t_packer *packer, Elf64_Phdr *a);
void		print_cave_size(Elf64_Phdr *a, Elf64_Phdr *b, t_packer *packer);
int8_t		print_error(char *self_path, char *error);
int			print_phdr_contain(t_packer *packer, int i);
void		print_symbol_code(void *data, size_t offset, size_t size);
int8_t		print_usage(char *self_path);
void		print_zones(t_list *zones);
void		rbt_free_content(void **content);
void		rbt_keep_content(void **content);
void		schedule_key(uint8_t *key, uint8_t *permutations);
ssize_t		solve_bytecodes(t_packer *packer,
			t_list *zones,
			t_zone *current_zone,
			t_btc *inst,
			int headless);
long long	t_rbt_compare_phdr(void *an, void *bn);
long long	t_rbt_compare_shdr(void *a, void *b);
void		test_cypher(void);
void		test_cypher_alter(void);
uint8_t		test_endian(void);
void		undo_update_zone(t_zone *zone, t_btc *inst);
void		unit_test_alter(uint8_t *content, size_t len);
void		unit_test_cypher(char *str, int len, char key);
void		update_arg_crypt_calls(t_btc *inst, t_zone *zone);
void		update_args(ssize_t ret, t_btc *inst);
void		update_zone(t_zone *zone, t_btc *inst);
void		write_btc(t_btc *inst, t_zone *zone, t_packer *packer);
void		write_decrypt_call(t_btc_args *arg);
void		write_decrypt_definition(t_btc_args *arg);
void		write_jump(t_btc_args *arg);
void		write_mem_rights(t_btc_args *arg);
void		write_woody(t_btc_args *arg);

#endif