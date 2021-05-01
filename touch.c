//
// Created by Jorge Goicoechea on 27/3/21.
//
#include <stdio.h>
#include <unistd.h>
#include<fcntl.h>
int main(int argc, char *argv[]){
    FILE *f;
    int i;
    int fd;
    for(i=1; i<argc; i++){
        fd=open(argv[i], O_CREAT, 0644);
        if(fd < 0) {
            perror("File already exists");
            goto OUT;
        }
    }
    return 0;
    OUT:
    return-1;
}
