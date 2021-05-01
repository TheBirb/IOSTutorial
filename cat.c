//
// Created by Jon Moríñigo on 16/3/21.
// Modified by Jon Moríñigo 1/05/21 /UPDATE_NOTE: Now cat can read multiple files and read from stdin!
//
#include<stdio.h>
#include<unistd.h> /*for STDOUT_FILENO*/
#include<stdlib.h>
#include<fcntl.h> /*for open*/
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
int isFile(const char* name){
    DIR* directory=opendir(name);
    if(directory!=NULL){
        closedir(directory);
        return 0;
    }
    if(errno==ENOTDIR){
        return 1;
    }
    return -1;
}
int main(int argc,char *argv[])
{

    if(argc==1){//User want's just stdin
       char buffer[512];
       scanf("%[^\n]%c", buffer); // we use [^\n]%c to get all the 512 byte line!
       printf("%s\n", buffer);
       return 0;
    }else{
        if(isFile(argv[1])==1){
            int fd, i, ch;
            for (i = 1; i < argc; i++) {                    //iterate the loop for total argument count.

                fd = open(argv[1],O_RDONLY);            //open the file in READONLY mode

                if(fd < 0) {
                    perror("open");
                    goto OUT;
                }

                while(read(fd,&ch,1))                   //Read one byte at a time

                    write(STDOUT_FILENO,&ch,1);     //Write the byte on Stdout

                close(fd);                              //close the file discriptor
            }
            printf("\n");
            return 0;
            OUT:
            return -1;
        }else{
            printf("Please enter a valid input\n");
        }

    }

}

