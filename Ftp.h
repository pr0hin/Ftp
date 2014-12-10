int sendAll(char * msg);

struct function {
    char * name;
    int (*cmd_func)(char *arr[]);
};

struct user {
    char *name;
    char *password;
};

void initialize_structs();

int user_cmd(char *arr[]);
int pass_cmd(char *arr[]);
int quit_cmd(char *arr[]);
int syst_cmd(char *arr[]);
int feat_cmd(char *arr[]);
int pwd_cmd(char*arr[]);
