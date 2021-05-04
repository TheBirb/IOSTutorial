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
    return -1;
}
//piping with different commands //supports different sizes! //created by Jon Moríñigo
int executePiping(int argc, char *argv[], int index){
    char instr1[200];
    char instr2[200];
    int iPipe= index;
    /*checking size*/
    if(iPipe==1 && argc==3){
        sprintf(instr1, "%s", argv[0]);
        sprintf(instr2, "%s %s",argv[3], argv[4]);
    }else if (iPipe==2 && argc==4){
        printf("here\n");
        sprintf(instr1, "%s %s",argv[0], argv[1]);
        sprintf(instr2, "%s %s",argv[2], argv[3]);
    }else if(iPipe==3 && argc==5){
        sprintf(instr1, "%s %s %s",argv[0], argv[1], argv[2]);
        sprintf(instr1, "%s", argv[4]);
    }else if(iPipe==1 && argc==4){
        sprintf(instr1, "%s", argv[0]);
        sprintf(instr2, "%s %s",argv[2], argv[3]);
    }else if(iPipe==2 && argc==5){
        sprintf(instr1, "%s %s",argv[0], argv[1]);
        sprintf(instr2, "%s %s",argv[3], argv[4]);
    }else if(iPipe==3 && argc==6){
        sprintf(instr1, "%s %s %s",argv[0], argv[1], argv[2]);
        sprintf(instr2, "%s %s",argv[5], argv[6]);
    }else if (iPipe==1 && argc==5){
        sprintf(instr1, "%s", argv[0]);
        sprintf(instr2, "%s %s %s",argv[2], argv[3], argv[4]);
    }else if(iPipe==2 && argc==6){
        sprintf(instr1, "%s %s",argv[0], argv[1]);
        sprintf(instr2, "%s %s %s",argv[3], argv[4], argv[5]);
    }else{
        sprintf(instr1, "%s %s %s",argv[0], argv[1], argv[2]);
        sprintf(instr2, "%s %s %s",argv[4], argv[5], argv[6]);
    }
    int saved_stdout = dup(1);
    int saved_stdin =dup(0);
    pid_t pid;
    int pipefd[2];
    pipe(pipefd);
    pid=fork();
    if(pid==0){
        close(pipefd[0]);
        dup2(pipefd[1], 1);
        system(instr1);
        exit(0);
    }else{

        pid=fork();
        if(pid==0){
            close(pipefd[1]);
            dup2(pipefd[0], 0);
            system(instr2);
            exit(0);

        }


    }

    dup2(saved_stdout, 1);
    close(saved_stdout);
    dup2(saved_stdin, 0);
    close(saved_stdin);
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
            for(int i=0; i<argc; i++){
                printf("%d.%s\n", i, args[i]);
            }
            if (argc > 2) {
                int index= indexOfPipe(argc, args);
                if (index != -1) {
                    executePiping(argc, args, index);

                } else {
                    execute(argc, args);

                }
            } else {
                execute(argc, args);

            }


        }else{

        }
        if (eof) exit(0);
    }
    return 0;
}
