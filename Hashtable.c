#include <stdio.h>
#include <stdlib.h>
#include "Ftp.h"
#include "Hashtable.h"

#define TABLE_SIZE 50
char *commands[34]={"USER", "PASS", "ACCT", "CWD", "CDUP", "SMNT", "QUIT", "REIN", "PORT", "PASV", "TYPE", "STRU", "MODE", "RETR", "STOR", "STOU", "APPE", "ALLO", "REST", "RNFR", "RNTO", "ABOR", "DELE", "RMD", "MKD", "PWD", "LIST", "NLST", "SITE", "SYST", "STAT", "HELP", "NOOP", "FEAT"};
struct function *hashtable[TABLE_SIZE];
int notinserted=0;
int inserted=0;

unsigned long hash2(char*key);

unsigned long hash(char * key, int iter) {
    unsigned long hash = 5381;
    int c;
    char i;
    //    while (c = *key++)

    while(i = *key++)
        hash = (int)i + (hash << 6) + (hash << 16) - hash;
    hash = hash ^ (int) i;  
    return hash + (iter * (hash2(key)));

}


unsigned long hash2(char*key) {
    unsigned long hash = 5381;
    int c;
    while (c = *key++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash; 
}

int indx(char * key, int iter) {
    return hash(key, iter) % TABLE_SIZE;
}

int insert(struct function *f, int index) {
    if (hashtable[index] == NULL) {

        hashtable[index] = f;
        return 0;
    }
    return -1;
}

//int tryhash() {
//    int i;
//    for (i = 0; i < 33; ++i) {
//        int j=0;
//        char * command = commands[i]; 
//        for (j = 0; j < 10; ++j) {
//            int in = indx(command, j);
//            int status = insert(command, in);
//            if (status == 0) {
//                printf("%s successfully inserted at: %d on %d iteration\n", command, in, j);
//                ++inserted;
//                break;
//            }
//            if (j == 9) {
//                printf("%s not inserted\n", command);
//                ++notinserted;
//
//            }
//        }
//    }
//    return 0;
//}

int retrieve(char * cmd) {
    int i, j;
        for (j = 0; j < 10; ++j) {
            i = indx(cmd, j);
            struct function *f = hashtable[i]; 
	    if (!f) {
		continue;
	    }
            int status;
            if ((status = strcmp(cmd, f->name)) == 0) {
                return i;
            }
        }
        return -1;
}
int initialize_hash_table(struct function *functions[]) {
    int i;
    for (i = 0; i < 6; ++i) {
        int j=0;
        struct function *f = functions[i]; 
        for (j = 0; j < 10; ++j) {
            int in = indx(f->name, j);
            int status = insert(f, in);
            if (status == 0) {
                printf("%s successfully inserted at: %d on %d iteration\n", f->name, in, j);
                ++inserted;
                break;
            }
            if (j == 9) {
                printf("%s not inserted\n", f->name);
                ++notinserted;

            }
        }
    }
    return 0;
}


//int main() {
//
//    tryhash();
//    printf("Inserted: %d\n", inserted);
//    printf("Notinserted: %d\n", notinserted);
//    return 0;
//}
