/*
 * =====================================================================================
 *
 *       Filename:  Ftp.c
 *
 *    Description:  Entry point for the FTP server
 *
 *        Version:  1.0
 *        Created:  12/07/2014 08:33:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Rohin Patel (), rohinjpatel@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include "Hashtable.h"
#include "Ftp.h"
#define LIMIT 160

int status;
int client;
char buf[LIMIT];
int bytes_received; 


int send_all(char * msg) {
    int total = 0;
    int len = strlen(msg);
    int bytes_left = len;
    int bytes_sent;
    while ( total < len) {
        bytes_sent = send(client, msg, len, 0);
        if (bytes_sent == -1) 
            break;
        total += bytes_sent;
    }
    return bytes_sent==-1? -1 : 0;
}

int handle_command(char *arr[]) {
    printf("Arr[0] is: %s\n", arr[0]);
    int indx = retrieve(arr[0]); 
    if (indx < 0) {
        char * msg = "502 Command not implemented\n";
        send_all(msg);
        return -1;
    }
    struct function *f = hashtable[indx];
    f->cmd_func(arr);
}

void free_array(char *arr[]){
    int i;
    for (i=0; i<5; ++i) {
        if (arr[i] != NULL) {
            free(arr[i]);
        }
    }
    return;
}
// split command into array at <SP> or <CRLF>
int parse_command(char * buf){
    int i;
    int n=0;
    char *args[5] = {NULL};
    char *nul = '\0';
    int len = strlen(buf);
    for (i=0; i<len; ++i) {
        if ((buf[i] == ' ') || ((buf[i] == '\r') && (buf[i+1] == '\n'))){
            char * arg = calloc(i+1, sizeof(char));
            strncpy(arg,&buf[0], i);	
            //(char*)mempcpy((arg+i),nul,1);
            args[n]=(char*)arg;
            ++n;
            buf = &buf[i+1];
            len = len - i;
            i=0;
        } 
        if (len <= 0) {
            break;
        }
    }
    printf("Command: %s\n", args[0]);
    int size = strlen(args[0]);
    printf("Arr[0] len: %d\n", size);
    printf("Command: %s\n", args[1]);
    printf("Command: %s\n", args[2]);
    handle_command(args);
    free_array(args);
    return 0;
}

void destroy_child_proc(int signo, siginfo_t *info, void *context) {
    int status;
    waitpid(info->si_pid,&status, 0);
}

int main (void) {
    initialize_structs();

    /* SET UP SIGNAL HANDLER FOR CHILDREN */
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = destroy_child_proc;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
    }

    /* SET UP SOCKETS */
    struct addrinfo localinfo, *res;
    struct sockaddr_storage their_addr;
    memset(&localinfo, 0, sizeof(localinfo));
    localinfo.ai_family = AF_UNSPEC;
    localinfo.ai_socktype = SOCK_STREAM;
    localinfo.ai_flags = AI_PASSIVE; // tells getaddr info to assign the address of localhost to the socket structure
    if ((status = getaddrinfo(NULL, "ftp", &localinfo, &res)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }
    // create a ipv4 TCP socket
    // create socket file descriptor
    int socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if ( socketfd  == -1) {
        fprintf(stderr, "socketfd error: %s\n", gai_strerror(socketfd));
        exit(2);
    }

    // bind socketfd to port
    if ( (status = bind(socketfd, res->ai_addr, res->ai_addrlen)) == -1) {
        fprintf(stderr, "bind error: %s\n", gai_strerror(status));
        exit(3);
    }
    if ( (status = listen(socketfd, 20)) == -1) {
        fprintf(stderr, "Not listening on socket %s\n", gai_strerror(status));
        exit(4);
    }

    /* KEEP LISTENING FOR NEW CLIENTS */
    while (1) {
        int addr_size = sizeof(their_addr);

        client = accept(socketfd, &their_addr, &addr_size); 
        if (client > 0) {
            pid_t child_pid = fork();
            // check if child was created by parent;
            if (child_pid == 0) {
                break;
            }
        }
    }
    char *msg = "220 Success Hello, this is your captain speaking!\n";
    send_all(msg);
    // receive the command
    if ((bytes_received = recv(client, &buf[0], LIMIT, 0)) == 0) {
        fprintf(stderr, "Receive error %s\n", gai_strerror(bytes_received));
        exit(6);
    }
    while (1) {
        parse_command(buf);
        memset(buf,'\0', 160);
        if ((bytes_received = recv(client, &buf[0], LIMIT, 0)) == 0) {
		// TODO need to deal with potential buffer overflow and making
		// sure that the entire packet is received.
            fprintf(stderr, "Receive error %s\n", gai_strerror(bytes_received));
            exit(6);
        }
    }
}
