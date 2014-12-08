#include <stdlib.h>
#include <string.h>
#include "Ftp.h"
#include "Hashtable.h"

void initialize_structs() {

struct function *functions[6];

struct function *user = (struct function *)malloc(sizeof(struct function));
user->name=(char *)malloc(sizeof(char)*4);
strcpy(user->name, commands[0]);
user->cmd_func=user_cmd;
functions[0] = user;

struct function *pass = (struct function *)malloc(sizeof(struct function));
pass->name=(char *)malloc(sizeof(char)*4);
strcpy(pass->name, commands[1]);
pass->cmd_func=pass_cmd;
functions[1] = pass;


struct function *quit = (struct function *)malloc(sizeof(struct function));
quit->name=(char *)malloc(sizeof(char)*4);
strcpy(quit->name, commands[6]);
quit->cmd_func=quit_cmd;
functions[2] = quit;

struct function *syst = (struct function *)malloc(sizeof(struct function));
syst->name=(char *)malloc(sizeof(char)*4);
strcpy(syst->name, commands[29]);
syst->cmd_func=syst_cmd;
functions[3] = syst;

struct function *feat = (struct function *)malloc(sizeof(struct function));
feat->name=(char *)malloc(sizeof(char)*4);
strcpy(feat->name, commands[33]);
feat->cmd_func=feat_cmd;
functions[4] = feat;

struct function *pwd = (struct function *)malloc(sizeof(struct function));
pwd->name=(char *)malloc(sizeof(char)*4);
strcpy(pwd->name, commands[25]);
pwd->cmd_func=pwd_cmd;
functions[5] = pwd;

initialize_hash_table(functions);

}
