// myShell0
//////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define error(a) {perror(a); exit(1);};
#define MAXLINE 200
#define MAXARGS 20
char programPath[200];
/////////// reading commands:

int read_args(int* argcp, char* args[], int max, int* eofp)
{
    static char cmd[MAXLINE];
    char* cmdp;
    int ret,i;

    *argcp = 0;
    *eofp = 0;
    i=0;
    while ((ret=read(0,cmd+i,1)) == 1) {
        if (cmd[i]=='\n') break;  // correct line
        i++;
        if (i>=MAXLINE) {
            ret=-2;        // line too long
            break;
        }
    }
    switch (ret)
    {
        case 1 : cmd[i+1]='\0';    // correct reading
            break;
        case 0 : *eofp = 1;        // end of file
            return 0;
            break;
        case -1 : *argcp = -1;     // reading failure
            fprintf(stderr,"Reading failure \n");
            return 0;
            break;
        case -2 : *argcp = -1;     // line too long
            fprintf(stderr,"Line too long -- removed command\n");
            return 0;
            break;
    }
    // Analyzing the line
    cmdp= cmd;

        for (i=0; i<max; i++) {  /* to show every argument */
            if ((args[i]= strtok(cmdp, " \t\n")) == (char*)NULL) break;
            cmdp= NULL;
        }

    if (i >= max) {
        fprintf(stderr,"Too many arguments -- removed command\n");
        return 0;
    }
    *argcp= i;
    return 1;
}

///////////////////////////////////////
//created by Jon Moríñigo
int indexOfPipe(int argc, char *argv[]){
    for(int i=0; i<argc; i++){
        if(strcmp(argv[i],"|")==0){
            return i;
        }
    }
}
//piping with different commands //supports different sizes! //created by Jon Moríñigo
int executePiping(int argc, char *argv[]){
    char instr1[200];
    char instr2[200];
    int iPipe= indexOfPipe(argc, argv);
    int type;
    /*checking size*/
    if(iPipe==1 && argc==3){
        type=1;
    }else if (iPipe==2 && argc==4){
        type=2;
    }else if(iPipe==3 && argc==5){
        type=3;
    }else if(iPipe==1 && argc==4){
        type=4;
    }else if(iPipe==2 && argc==5){
        type=5;
    }else if(iPipe==3 && argc==6){
        type=6;
    }else if (iPipe==1 && argc==5){
        type=7;
    }else if(iPipe==2 && argc==6){
        type=8;
    }else{
        type=9;
    }
    //piping
    pid_t pid;
    int fd[2];
    if(type==1){

        sprintf(instr2, "%s %s",argv[3], argv[4]);
        pipe(fd);
        pid=fork();
        if(pid==0){
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            system(argv[0]);
        }else{
            pid=fork();
            if(pid==0){
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);
                system(argv[2]);
            }
            ;
        }
    }else if(type==2) {
        sprintf(instr1, "%s %s",argv[0], argv[1]);
        sprintf(instr2, "%s %s",argv[2], argv[3]);
        pipe(fd);
        pid=fork();
        if(pid==0){
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            system(instr1);
        }else{
            pid=fork();
            if(pid==0){
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);
                system(instr2);
            }
            ;
        }
    }else if(type==3) {
        sprintf(instr1, "%s %s %s",argv[0], argv[1], argv[2]);

        pipe(fd);
        pid=fork();
        if(pid==0){
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            system(instr1);
        }else{
            pid=fork();
            if(pid==0){
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);
                system(argv[4]);
            }
            ;
        }
    }else if(type==4){
        sprintf(instr2, "%s %s",argv[2], argv[3]);
        pipe(fd);
        pid=fork();
        if(pid==0){
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            system(argv[0]);
        }else{
            pid=fork();
            if(pid==0) {
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);
               system(instr2);
            }
        }
    }else if(type==5) {
        pipe(fd);
        pid=fork();
        if(pid==0){
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            system(instr5);
        }else{
            pid=fork();
            if(pid==0) {
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);
                system(instr2);
            }
        }
    }else if(type==6){
        sprintf(instr1, "%s %s, %s",argv[0], argv[1], argv[2]);
        sprintf(instr2, "%s %s",argv[5], argv[6]);
        pipe(fd);
        pid=fork();
        if(pid==0){
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            system(instr1);
        }else{
            pid=fork();
            if(pid==0){
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);
                system(instr2);
            }
            ;
        }
    }else if(type==7){
        sprintf(instr2, "%s %s %s",argv[2], argv[3], argv[4]);
        pipe(fd);
        pid=fork();
        if(pid==0){
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            system(argv[0]);
        }else{
            pid=fork();
            if(pid==0){
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);
                system(instr2);
            }

        }
    }else if(type==8){
        sprintf(instr1, "%s %s",argv[0], argv[1]);
        sprintf(instr2, "%s %s %s",argv[3], argv[4], argv[5]);
        pipe(fd);
        pid=fork();
        if(pid==0){
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            system(intr1);
        }else{
            pid=fork();
            if(pid==0){
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);
                system(instr2);
            }
            ;
        }
    }else{
        sprintf(instr1, "%s %s %s",argv[0], argv[1], argv[2]);
        sprintf(instr2, "%s %s %s",argv[4], argv[5], argv[6]);
        pipe(fd);
        pid=fork();
        if(pid==0){
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            system(instr1);
        }else{
            pid=fork();
            if(pid==0){
                dup2(fd[0], STDIN_FILENO);
                close(fd[0]);
                system(instr2);
            }
            ;
        }
    }
    return 0;
}
int execute(int argc, char *argv[])
{
    char currentDir[200];
    if(strcmp(argv[0], "exit")==0){
        exit(0);
    }else if(strcmp(argv[0], "cd")==0) {
        if(argc<2){
            printf("Need more parameter, Ex: 'cd root'\n");
        }else{
            if (chdir(argv[1])==-1){
                char *error="cd:The directory doesn't exist\n";
                write(1,error, strlen(error));
                return -1;
            }else{
                return 0;
            }
        }
    }else{

        getcwd(currentDir, sizeof(currentDir));
        sprintf(currentDir, "%s%s", programPath,argv[0]);
        int pid=fork();
        if(pid==0){
            execvp(currentDir, argv);
        }else{
            wait(NULL);
        }
    }

    return 0;

}

int main ()
{
    getcwd(programPath, sizeof programPath);
    sprintf(programPath, "%s/", programPath);

    char * Prompt = "myShell0> ";
    int eof= 0;
    int argc;
    char *args[MAXARGS];

    while (1) {
        write(0,Prompt, strlen(Prompt));
        if (read_args(&argc, args, MAXARGS, &eof) && argc > 0) {
            if(argc>2){
                if(strcmp(args[2], "|")==0){
                    executePiping(argc, args);
                }else{
                    execute(argc, args);
                }
            }else{
                execute(argc, args);
            }


        }else{

        }
        if (eof) exit(0);
    }
    return 0;
}
