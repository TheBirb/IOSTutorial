//
// Created by Jon Moríñigo on 30/3/21.
//
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <fcntl.h>

int main(int argc , char *argv[])
{
    int fd;
    FILE *fp;
    char line[300];
    // initialsing the file pointer to read
    fd=open(argv[2], O_RDONLY);
    if(fd<0){
        perror("open");
        goto OUT;
    }
    fp = fopen(argv[2],"r");

    // reading line by line and comparing each word in line
    while(fscanf(fp , "%[^\n]\n" , line)!=EOF)
    {
        if(strstr(line , argv[1]) !=NULL)
        {
            // print that line
            printf("%s\n" , line);
        }
        else
        {
            continue;
        }
    }
    fclose(fp);
    return 0;
    OUT:
    return -1;
}