all: sim_cache.c
	gcc -g -Wall -o sim_cache sim_cache.c hex_to_bin.c add_zeros.c parse.c binary_tree.c cache_process.c

clean:
	$(RM) sim_cache
