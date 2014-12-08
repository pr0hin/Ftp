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
struct function user;
struct function pass;
//struct function acct;
struct function cwd;
struct function cdup;
struct function smnt;
struct function quit;
struct function rein;
struct function port;
struct function pasv;
struct function type;
struct function stru;
struct function mode;
struct function retr;
struct function stor;
struct function stou;
struct function appe;
struct function allo;
struct function appe;
struct function rest;
struct function rnfr;
struct function abor;
struct function dele;
struct function rmd;
struct function mkd;
struct function pwd;
struct function list;
struct function nlst;
struct function site;
struct function syst;
struct function stat;
struct function help;
struct function noop;
