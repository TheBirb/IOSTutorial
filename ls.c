//
// Created by Jon Moríñigo on 17/3/21.
//
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include<stdlib.h>
#include<dirent.h>
#include<time.h>
#include<pwd.h>
#include <grp.h>
#include <string.h>
#include <errno.h>
int main(int argc, char *argv[]){
    DIR *dir;
    struct dirent *currentDir; //current directory
    struct stat thestat;      //file stat
    struct passwd *owner; //owner
    struct group *group; //group
    char *buf;
    char *c;
    int i;
    int type;
    char currentPath[200];
    char programPath[200];

    if(argc <1) {

        exit(EXIT_FAILURE);

    }else if(argc==1){
        if((dir=opendir("."))==NULL){
            perror("Couldn't open '.'");
            return -1;
        }

        type=1;
    }else if(argc==2){
        if(strcmp("-l", argv[1])==0){
            if((dir=opendir("."))==NULL) {
                perror("Couldn't open '.'");
                return -1;
            }
            type=2;
        }else {
            if ((dir = opendir(argv[1])) == NULL) {
                perror("Couldn't open directory");


                return -1;
            }

            type = 1;
        }
    }else{

        if((dir=opendir(argv[2]))==NULL){
            printf("%s\n", strerror(errno));
            perror("Couldn't open directory");
            return -1;
        }

        type=3;

    }

    if(type==1){
        while((currentDir=readdir(dir))!=NULL){
            printf("%s\n",currentDir->d_name);
        }
    }else{
        if(type==3){

            getcwd(currentPath, sizeof currentPath);
            sprintf(currentPath, "%s/%s/", currentPath, argv[2]);
        }
        while((currentDir=readdir(dir))!=NULL){

            buf = currentDir->d_name;

            if(strcmp(buf, ".")==0 ||strcmp(buf, "..")==0 ){

            }else{
                if(type==3){
                    sprintf(programPath, "%s%s", currentPath, buf);
                    stat(programPath, &thestat);
                }else{
                    stat(buf, &thestat);
                }


                switch (thestat.st_mode & S_IFMT) {
                    case S_IFBLK:  printf("b"); break;
                    case S_IFCHR:  printf("c"); break;
                    case S_IFDIR:  printf("d"); break; //It's a (sub)directory
                    case S_IFIFO:  printf("p"); break; //fifo
                    case S_IFLNK:  printf("l"); break; //Symbolic link
                    case S_IFSOCK: printf("s"); break;
                        //Filetype isn't identified
                    default:       printf("-"); break;
                }

                //[permissions]
                //Same for the permissions, we have to test the different right
                printf( (thestat.st_mode & S_IRUSR) ? "r" : "-");
                printf( (thestat.st_mode & S_IWUSR) ? "w" : "-");
                printf( (thestat.st_mode & S_IXUSR) ? "x" : "-");
                printf( (thestat.st_mode & S_IRGRP) ? "r" : "-");
                printf( (thestat.st_mode & S_IWGRP) ? "w" : "-");
                printf( (thestat.st_mode & S_IXGRP) ? "x" : "-");
                printf( (thestat.st_mode & S_IROTH) ? "r" : "-");
                printf( (thestat.st_mode & S_IWOTH) ? "w" : "-");
                printf( (thestat.st_mode & S_IXOTH) ? "x" : "-");
                printf(" ");

                // [number of hard links]
                // "This count keeps track of how many directories have entries for this file
                printf("%d ", thestat.st_nlink);

                //[owner]
                owner = getpwuid(thestat.st_uid);
                printf("%s ", owner->pw_name);
                //[group]
                group = getgrgid(thestat.st_gid);
                printf("%s ", group->gr_name);

                //size, name and date
                printf("%4d",thestat.st_size);
                char *c=ctime(&thestat.st_mtime);
                printf(" ");
                for(i=4; i<15; i++){
                    printf("%c", c[i]);
                }
                printf(" ");
                printf("%s\n", currentDir->d_name);

            }



        }
    }

    closedir(currentDir);
    return 0;
}
