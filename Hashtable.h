char *commands[];
struct function *functions[];
struct function *hashtable[];

int initialize_hash_table(struct function *functions[]);
unsigned long hash (char * key, int iter);

int indx(char * key, int iter);

int tryhash();
