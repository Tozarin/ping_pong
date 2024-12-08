#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void pipe_read(int pipe_in, char* msg) {
    read(pipe_in, msg, sizeof(msg));
}

void pipe_write(int pipe_out, char* msg) {
    write(pipe_out, msg, sizeof(msg));
}

char* choose_answer(char* msg) {

    if (strcmp(msg, "PING") == 0) {
        return "PONG";
    } else {
        return "PING";
    }
}

void loop(int pipe_in, int pipe_out) {

    while (true) {
        char msg[4];
        pipe_read(pipe_in, msg);

        sleep(1);

        printf("%d %s\n", getpid(), msg);

        char* answ = choose_answer(msg);
        pipe_write(pipe_out, answ);
    }
}

int main() {

    int p2c_pipes[2];
    int c2p_pipes[2];

    if (pipe(p2c_pipes) != 0) {
        perror("Error while creating pipe");
        exit(1);
    }

    if (pipe(c2p_pipes) != 0) {
        perror("Error while creating pipe");
        exit(1);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("Error while forking");
        exit(1);
    }

    if (pid == 0) {
        char msg[4] = "PING";
        pipe_write(c2p_pipes[1], msg);
        
        loop(p2c_pipes[0], c2p_pipes[1]);
    } else {
        loop(c2p_pipes[0], p2c_pipes[1]);
    }

    return 0;
}