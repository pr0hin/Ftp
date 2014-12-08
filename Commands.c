#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>
#include "Ftp.h"
struct user anonymous = { .name = "anonymous", .password=""};
struct user valid_users[2] = {
    {"rohin", "12345"},
    {"anonymous", ""}
};
char *current_user;
char current_dir[1024];


int user_cmd(char *arr[]) {
    int res;
    char * cmd = "USER";
    if ((res = strcmp(arr[0], cmd)) != 0) {
        return -1;
    }
    int i;
    struct user u;
    int len;
    for (i=0; i < 2; ++i) {
        u = valid_users[i];
        len = strlen(u.name);
        if ((res = strcmp(arr[1], u.name)) == 0){
            current_user = (char *) malloc (len * sizeof(char));
            strcpy(current_user, arr[1]); 
            if (arr[2] == NULL) {  
                char * msg = "331 User name okay, need password\n"; 
                send_all(msg);
                return 0;
            }

            if ((res = strcmp(arr[2], u.password)) == 0 ){
                char * msg = "230 User logged in, proceed\n";
                send_all(msg);
                return 0;
            }
        }
    }
    
    char * msg = "430 Invalid username\n";
    send_all(msg);
    return -1;
}


// needs to know what user to compare password to
int pass_cmd(char *arr[]) {
    int res;
    char * cmd = "PASS";
    if ((res = strcmp(arr[0], cmd)) != 0) {
        return -1;
    }
    int i;
    for (i=0; i < 2; ++i) {
        struct user u = valid_users[i];
        res = strcmp(current_user, u.name);
        if (res == 0) { 
            if ((res = strcmp(arr[1], u.password) == 0))  {
                char * msg = "230 User logged in, proceed\n";
                send_all(msg);
                return 0;
            } else {
                char * msg = "430 incorrect password\n";
                send_all(msg);
                return -1;
            }
        }
    }
// TODO need to put error message
    return -1;
}

int syst_cmd(char * arr[]) {
    int res;
    char * cmd = "SYST";
    if ((res = strcmp(arr[0], cmd)) != 0) {
        return -1;
    }
    char * msg = "215 Unix System Type\n";
    send_all(msg);
    return 0;
}

int feat_cmd(char* arr[]) {
    int res;
    char * cmd = "FEAT";
    if ((res = strcmp(arr[0], cmd)) != 0) {
        return -1;
    }
    char * msg = "211 No Features\n";
    send_all(msg);
    return 0;


}
int pwd_cmd(char *arr[]) {
    int res;
    char * cmd = "PWD";
    if ((res = strcmp(arr[0], cmd)) != 0) {
        return -1;
    }
    send_all("257 ");
    memset(current_dir, '\0', 1024);
    getcwd(current_dir, sizeof(current_dir));
    send_all(current_dir);
    send_all("\n");
}
    
int quit_cmd(char * arr[]) {
    int res;
    char * cmd = "QUIT";
    if ((res = strcmp(arr[0], cmd)) != 0) {
        return -1;
    }
    char * msg = "221, Goodbye\n";
    send_all(msg);
    
    free(current_user);
    kill(getpid(), SIGTERM);
    return 0;
}

