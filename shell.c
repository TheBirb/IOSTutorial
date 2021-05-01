// myShell0
//////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <termios.h>


#define error(a) {perror(a); exit(1);};
#define MAXLINE 200
#define MAXARGS 20
char programPath[200];
int stage;
int scene;
int done;
char actualPath[200];
char *Prompt;
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
//to check where the pipe is
int indexOfPipe(int argc, char *argv[]){
    for(int i=0; i<argc; i++){
        if(strcmp(argv[i],"|")==0){
            return i;
        }
    }
}
//piping with different commands //supports different sizes! ONLY SUPPORTS ONE PIPE! //created by Jon Moríñigo
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
            system(instr1);
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
       return 0;
    }else if(strcmp(argv[0], "cd")==0) {
        if(argc<2){
            printf("Need more parameter, Ex: 'cd root'\n");
        }else{
            if (chdir(argv[1])==-1){
                char *error="cd:The directory doesn't exist";
                write(1,error, strlen(error));
                return -1;
            }else{
                return 0;
            }
        }
    }else{
        printf("%s\n", argv[0]);
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
int enterDetector(){
    char *m="Press enter to continue...\n";
    write(0, m, strlen(m));
    while(getchar()!='\n'){
        ;
    }
    return 0;

}
int executeCommandPrompt(){
    char *Prompt="SpellBook$ ";
    int eof= 0;
    int argc;
    char *args[MAXARGS];
    while (1) {
        write(0,Prompt, strlen(Prompt));
        if (read_args(&argc, args, MAXARGS, &eof) && argc > 0) {
            if(stage==0){ //tutorial
                switch(scene){
                    case 0:
                        if(strcmp(args[0], "ls")!=0){
                            return 1;
                        }else if(argc>1){
                            return 3;
                        }else{
                            done=0;
                        }
                        break;
                    case 1:
                        if(argc<2){
                            printf("Write well the instruction!\n");
                            return 4;
                        }else if(strcmp(args[0], "ls")!=0){
                            return 1;
                        }else if(strcmp(args[1], "TutorialZone")){
                            return 2;
                        }else if(argc>2){
                            return 3;
                        }else{
                            done=0;
                        }
                        break;
                    case 2:
                        if(argc<2){
                            printf("Write well the instruction!\n");
                            return 4;
                        }else if(strcmp(args[0], "ls")!=0){
                            return 1;
                        }else if(strcmp(args[1], "-l")){
                            return 1;
                        }else if(argc>2){
                            return 3;
                        }else{
                            done=0;
                        }
                        break;
                    case 3:
                        if(argc<3){
                            printf("Write well the instruction!\n");
                            return 4;
                        }else if(strcmp(args[0], "ls")!=0){
                            return 1;
                        }else if(strcmp(args[1], "-l")) {
                            return 1;
                        }else if(strcmp(args[2], "TutorialZone")){
                            return 2;
                        }else if(argc>3){
                            return 3;
                        }else{
                            done=0;
                        }
                        break;
                    case 4:
                        if(argc<2){
                            printf("Write well the instruction!\n");
                            return 4;
                        }else if(strcmp(args[0], "cd")!=0){
                            return 1;
                        }else if(strcmp(args[1], "TutorialZone")){
                            return 2;
                        }else{
                            done=0;
                        }
                        break;
                    case 5:
                        if(strcmp(args[0], "pwd")!=0){
                            return 1;
                        }else if(argc>1){
                            return 3;
                        }else{
                            done=0;
                        }
                        break;
                    case 6:
                        if(argc<2){
                            printf("Write well the spell!\n");
                            return 4;
                        }else if(strcmp(args[0],"cat")!=0){
                            return 1;
                        }else if(strcmp(args[1],"VirusLv1")!=0){
                            return 2;
                        }else if(argc>2){
                            return 3;
                        }else{
                            done=0;
                        }
                        break;
                    case 7:
                        if(argc<3){
                            printf("Write well the instruction!\n");
                            return 4;
                        }else if(strcmp(args[0], "grep")!=0){
                            return 1;
                        }else if(strcmp(args[1], "Darius")!=0){
                            return 2;
                        }else if(strcmp(args[2], "VirusLv1")!=0){
                            return 2;
                        }else if(argc>3){
                            return 3;
                        }else{
                            done=0;
                        }
                        break;
                    case 8:
                        if(argc<2){
                            printf("Write well the instruction!\n");
                            return 4;
                        }else if(strcmp(args[0], "touch")!=0) {
                            return 1;
                        }else if(argc>2){
                            return 3;
                        }else{
                            done=0;
                        }
                        break;
                    case 9:
                        if(argc<1){
                            printf("Write well the instruction!\n");
                            return 4;
                        }else if(strcmp(args[0], "ls")!=0) {
                            return 1;
                        }else if(argc>1){
                            return 3;
                        }else{
                            done=0;
                        }
                        break;
                    case 10:
                        if(argc<3){
                            printf("Write well the instruction!\n");
                            return 4;
                        }else if(strcmp(args[0], "mv")!=0){
                            return 1;
                        }else if(strcmp(args[1], "fireball")!=0){
                            return 2;
                        }else if(strcmp(args[2], "Trash")!=0){
                            return 2;
                        }else if(argc>3){
                            return 3;
                        }else{
                            done=0;
                        }
                        break;
                    case 11:
                        if(argc<3){
                            printf("Write well the instruction!\n");
                            return 4;
                        }else if(strcmp(args[0], "cp")!=0){
                            return 1;
                        }else if(strcmp(args[1],"fireball")!=0){
                            return 2;
                        }else if(strcmp(args[2], "fireball2")!=0){
                            return 2;
                        }else if(argc>3){
                            return 3;
                        }else{
                            done=0;
                        }
                    default:
                        ;
                }
            }else if(stage==2){}
                if(strcmp(args[0], "cd")==0 ||strcmp(args[0], "ls")==0) {
                    ;
                }else {
                    printf("You can't use those spells here!");
                    return 0;
                }
            }else if(stage==3) {
            if (strcmp(args[0], "cd") == 0 || strcmp(args[0], "ls") == 0 || strcmp(args[0], "cat") == 0) {
            } else {
                printf("You can't use those spells here!");
                return 0;
            }

        }
        execute(argc, args);
        if (eof) break;
        if(done==0) break;
    }
    return 0;
}
//password utils
void setTermNoCanon(struct termios *savedtm){
    struct termios tm;
    tcgetattr(0, &tm);
    tcgetattr(0, savedtm);
    tm.c_lflag &=~(ICANON|ECHO);
    tm.c_cc[VMIN]=1;
    tm.c_cc[VTIME]=0;
    tcsetattr(0, TCSANOW, &tm);
}
void recover_tm(struct termios *tm){
    tcsetattr(0, TCSANOW, tm);
}

void tutorialStage() {
    char *warning1 = "TUTORIAL:Write the instruction correctly!\n"; //instruction is not the expected one
    char *warning2 = "TUTORIAL:Write the directory correctly!\n";  // directory is no the expected one
    char *warning3 = "TUTORIAL: You dont need to put anything else!\n"; //more arguments than expected
    char *question;
    char *answer;
    int i;
    //Intro
    char *message = "  ____          _      _                    _ \n";
    write(1, message, strlen(message));
    message = " / ___|___   __| | ___| |    __ _ _ __   __| |\n";
    write(1, message, strlen(message));
    message = "| |   / _ \\ / _` |/ _ \\ |   / _` | '_ \\ / _` |\n";
    write(1, message, strlen(message));
    message = "| |__| (_) | (_| |  __/ |__| (_| | | | | (_| |\n";
    write(1, message, strlen(message));
    message = " \\____\\___/ \\__,_|\\___|_____\\__,_|_| |_|\\__,_|;\n";
    write(1, message, strlen(message));
    message = "[DARIUS]: Welcome to codeland a place where you will learn some basic linux commands!\n";
    write(1, message, strlen(message));
    enterDetector();
    message = "[DARIUS]: Firstly, let me introduce myself, I'm Darius, your Artificial Assistant, I will teach you the basic commands also know in our world\n";
    write(0, message, strlen(message));
    message = "as spells. Yes you have read correctly, in this world linux commands are used as spells to defeat monsters\n";
    write(0, message, strlen(message));
    enterDetector();
    question = "[DARIUS]: Is there something you want to know about this world?\n";
    write(0, question, strlen(question));
    message = "TUTORIAL:To select a question you must write the number of the question and hit enter!\n";
    answer = "1:How is this world called?\n";
    write(0, answer, strlen(answer));
    answer = "2:I have no questions, let's continue\n";
    write(1, answer, strlen(answer));
    write(1, message, strlen(message));
    char select[1];
    read(1, select, sizeof select);
    if (strcmp(select, "1") == 0) {
        message = "[DARIUS]:  This world is called CodeLand!\n";
        write(1, message, strlen(message));
        message = "[DARIUS]: I'm so excited for you being here! Let's go straight to the basics of wizardry\n";
        write(1, message, strlen(message));
    } else {
        message = "[DARIUS]: As I see you don't wan't to know anything else, let's start with the basics\n";
        write(1, message, strlen(message));
    }
    //stage==0--------------------------------------------------------------------------------------
    //scene 0 ls
    scene = 0;
    message = "[DARIUS]: The first thing we need to do is locate the Tutorial zone\n";
    write(0, message, strlen(message));
    message = "[DARIUS]: To do this we use a spell called 'ls' to see all the files that are in the same directory\nYou just need to type 'ls' to search the actual zone you are in!\n";
    write(0, message, strlen(message));
    message = "[DARIUS]: Let's put it into practice!\n";
    write(0, message, strlen(message));
    enterDetector();

    i = 4;
    done = 1;
    while (i != 0) {
        i = executeCommandPrompt();
        if (i == 1) {
            write(0, warning1, strlen(warning1));
        } else if (i == 3) {
            write(0, warning2, strlen(warning3));
        }
    }
    //scene 1   ls in another directory
    scene = 1;
    message = "[DARIUS]: Well done!, As you can see above we can see all the files in the same directory and we can also see the TutorialZone!\n";
    write(1, message, strlen(message));
    enterDetector();
    message = "[DARIUS]: We can use this spell to search inside zones we are not in! What a powerful spell!!!!\n";
    write(1, message, strlen(message));
    enterDetector();
    message = "[DARIUS]: Lets try it! Write 'ls TutorialZone' and watch what happens!\n";
    write(1, message, strlen(message));
    enterDetector();
    i = 4;
    done = 1;
    while (i != 0) {
        i = executeCommandPrompt();
        if (i == 1) {
            write(0, warning1, strlen(warning1));
        } else if (i == 2) {
            write(0, warning2, strlen(warning2));
        }
    }
    message = "[DARIUS]: As you can see, we now know about all the files that are in the same directory\n The '.' entry refers to the actual directory we are in and the '..' is the directory where the directory we are now on is\n It is also known as the 'parent' directory\n";
    write(1, message, strlen(message));
    enterDetector();
    //scene 2 ls -l
    scene = 2;
    message = "[DARIUS]: There is an alteration of the spell 'ls' that can give us a lot more information of the files\n";
    write(1, message, strlen(message));
    message = "[DARIUS]: This spell is know as 'ls -l' and tells us information about a file or directory, like who is the owner of the file, the date the file was created, the size, etc.\n";
    write(1, message, strlen(message));
    message = "[DARIUS]: Let's try it\n";
    write(1, message, strlen(message));
    enterDetector();
    i = 4;
    done = 1;
    while (i != 0) {
        i = executeCommandPrompt();
        if (i == 1) {
            write(1, warning1, strlen(warning1));
        } else if (i == 3) {
            write(1, warning2, strlen(warning3));
        }
    }
    //scene 3 ls -l to another
    scene = 3;
    message = "[DARIUS]: So, as I said before, the 'ls -l' spell is an alteration, this means we can use it also to see what is inside other directories.\n";
    write(1, message, strlen(message));
    enterDetector();
    message = "[DARIUS]: Let's do 'ls -l TutorialZone' and see what's the result!\n";
    write(1, message, strlen(message));
    enterDetector();
    i = 4;
    done = 1;
    while (i != 0) {
        i = executeCommandPrompt();
        if (i == 1) {
            write(1, warning1, strlen(warning1));
        } else if (i == 3) {
            write(1, warning2, strlen(warning3));
        }
    }
    //scene 4 cd
    message = "[DARIUS]: As I told you before, now we can see in detail the characteristics of the files in that directory!\n";
    write(1, message, strlen(message));
    enterDetector();
    scene = 4;
    message = "[DARIUS]: Now we need to travel to the TutorialZone!\n";
    write(1, message, strlen(message));
    enterDetector();
    message = "[DARIUS]: To do this, write on the command prompt cd and put the directory or folder you want to switch to\n";
    write(1, message, strlen(message));
    enterDetector();
    message = "[DARIUS]: Remember to put an space between the cd and the directory!!\n";
    write(1, message, strlen(message));
    enterDetector();
    i = 4;
    done = 1;
    while (i != 0) {
        i = executeCommandPrompt();
        if (i == 1) {
            write(1, warning1, strlen(warning1));
        } else if (i == 2) {
            write(1, warning2, strlen(warning2));
        }
    }
    //scene 5 pwd
    scene = 5;
    message = "[DARIUS]: Nicely done! Now you are in the tutorial zone\n";
    write(1, message, strlen(message));
    enterDetector();
    message = "[DARIUS]: And you might think, how does Darius know we are already in the tutorialZone?\n";
    write(1, message, strlen(message));
    enterDetector();
    message = "[DARIUS]: Well, there is a spell know as 'pwd' that tells us where we are\n";
    write(1, message, strlen(message));
    enterDetector();
    message = "[DARIUS]: Let's try it!\n";
    write(1, message, strlen(message));
    done = 1;
    enterDetector();
    i = 4;
    while (i != 0) {
        i = executeCommandPrompt();
        if (i == 1) {
            write(1, warning1, strlen(warning1));
        } else if (i == 2) {
            write(1, warning2, strlen(warning2));
        } else if (i == 3) {
            write(1, warning3, strlen(warning3));
        }
    }
    //scene 6 cat
    scene = 6;
    message = "[DARIUS]: The last string after the last '/' is where you are currently in, and the others at the left are their parents\n";
    write(1, message, strlen(message));
    enterDetector();
    message = "[DARIUS]: In this tutorial place we have a level one Virus that I have already weakened\n";
    write(1, message, strlen(message));
    enterDetector();
    message = "[DARIUS]: To see the contents of this virus and to see that I have already left my mark in this virus we can use the spell grep\n";
    write(1, message, strlen(message));
    enterDetector();
    message = "[DARIUS]: To do this there is a spell called 'cat', we can use it to see al the contents of a file\n";
    write(1, message, strlen(message));
    enterDetector();
    message = "[DARIUS]: Let's use it on the Virus! Write 'cat VirusLv1'!\n";
    write(1, message, strlen(message));
    enterDetector();
    i = 4;
    done = 1;
    while (i != 0) {
        i = executeCommandPrompt();
        if (i == 1) {
            write(1, warning1, strlen(warning1));
        } else if (i == 2) {
            write(1, warning2, strlen(warning2));
        } else if (i == 3) {
            write(1, warning3, strlen(warning3));
        }
    }
    //scene 7 grep
    scene = 7;
    message = "[DARIUS]: Well! As you can see it is pretty difficult to search for the marks I left!\n";
    write(1, message, strlen(message));
    enterDetector();
    message = "[DARIUS]: Oh! Don't worry! The wizards of these lands thought of a better way to search for things!\n There is a spell called 'grep' used to do this\n";
    write(1, message, strlen(message));
    enterDetector();
    message = "[DARIUS]: Just write 'grep Darius VirusLv1' and see the magic!\n";
    write(1, message, strlen(message));
    enterDetector();
    i = 4;
    done = 1;
    while (i != 0) {
        i = executeCommandPrompt();
        if (i == 1) {
            write(1, warning1, strlen(warning1));
        } else if (i == 2) {
                write(1, warning2, strlen(warning2));
        } else if (i == 3) {
            write(1, warning3, strlen(warning3));
        }
    }
    chdir("TutorialZone");
    message = "[DARIUS]: As you can see, with grep we can search for things quickly instead of having to read all the virus!\n";
    write(1, message, strlen(message));
    //scene 8 mv
    scene = 8;
    message = "[DARIUS]: Okey, let's go on with the next spell, this one is called touch,\n"
              "with touch you can create new files!\n";
    write(1, message, strlen(message));
    message = "[DARIUS]: Let's try it!, Write touch and the name of the file, let's name it fireball!\n";
    write(1, message, strlen(message));
    enterDetector();
    i = 4;
    done = 1;
    while (i != 0) {
        i = executeCommandPrompt();
        if (i == 1) {
            write(1, warning1, strlen(warning1));
        } else if (i == 3) {
            write(1, warning3, strlen(warning3));
        }
    }
    scene = 9;
    message="[DARIUS]: Now if you do ls you will see the new file was created!\n";
    write(1, message, strlen(message));
    enterDetector();
    i = 4;
    done = 1;
    while (i != 0) {
        i = executeCommandPrompt();
        if (i == 1) {
            write(1, warning1, strlen(warning1));
        } else if (i == 2) {
            write(1, warning2, strlen(warning2));
        } else if (i == 3) {
            write(1, warning3, strlen(warning3));
        }
    }
    scene=11;
    message="[DARIUS]: OK, now we are in the Trash. Yes literally, but try 'cp fireball fireball2\n";
    write(1, message, strlen(message));
    enterDetector();
    i = 4;
    done = 1;
    while (i != 0) {
        i = executeCommandPrompt();
        if (i == 1) {
            write(1, warning1, strlen(warning1));
        } else if (i == 2) {
            write(1, warning2, strlen(warning2));
        } else if (i == 3) {
            write(1, warning3, strlen(warning3));
        }
    }
    scene=0;
    message="[DARIUS]: Now do ls :)\n";
    write(1, message, strlen(message));
    enterDetector();
    i = 4;
    done = 1;
    while (i != 0) {
        i = executeCommandPrompt();
        if (i == 1) {
            write(0, warning1, strlen(warning1));
        } else if (i == 3) {
            write(0, warning2, strlen(warning3));
        }
    }
    message="[DARIUS]: and MAGIC!!!! Now we have to copies of fireball";
    write(1, message, strlen(message));
    enterDetector();
    scene=10;
    message="[DARIUS]: But right now we don't want that there, so let's throw it to the trash to do this we have a spell to move things!\n";
    write(1, message, strlen(message));
    enterDetector();
    message="[DARIUS]: Yes! You've read correctly! Use the spell 'mv' to move fireball to the trash!\n"
            "[DARIUS]: Just type 'mv fireball Trash' and see the MAGIC!\n";
    write(1, message, strlen(message));
    enterDetector();
    i = 4;
    done = 1;
    while (i != 0) {
        i = executeCommandPrompt();
        if (i == 1) {
            write(1, warning1, strlen(warning1));
        } else if (i == 2) {
            write(1, warning2, strlen(warning2));
        } else if (i == 3) {
            write(1, warning3, strlen(warning3));
        }
    }
    message="[DARIUS]: Now the file is in the trash and it will eventually disappear!\n";
    write(1, message, strlen(message));
    enterDetector();
    chdir("Trash");
    system("rm fireball");
    chdir("..");
    system("rm fireball2");
    scene=12;
    message="[DARIUS]: And a little tip! You can use man [command] or help [command] to see or remember how commands work ;)\n ";
    write(1, message, strlen(message));
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    message="[DARIUS]: WOW! You've come so far! But I have some bad news!\n"
            "[DARIUS]: I've got a lot of student's they a lot of them didn't deserve to enter the real CodeLand\n"
            "[DARIUS]: Yes! All you you have experienced was a simulation of CodeLand\n"
            "[DARIUS]: Now, YOU have to prove me you have learned >:)\n";
    write(1, message, strlen(message));
    message ="[DARIUS]: The password for the test is 'DARIUSFINALTEST'\n";
    write(1, message, strlen(message));
    enterDetector();

}
void dariusSpellTest(){
    chdir("DariusSpellTest");
   char* message="[DARIUS]: WELCOME!!!!!\n";
    write(1, message, strlen(message));
    message="[DARIUS]: Let's start directly with the test!\n";
    write(1, message, strlen(message));
    char answer[100];
    message="[DARIUS]:Which spell do we use to see the contents of a file?\n";
    write(1, message, strlen(message));
    while(1){
        scanf("%s",answer);
        if(strcmp(answer, "cat")==0){
            printf("Correct!\n");
            break;
        }else{
            printf("Incorrect! Try again!");
        }
    }
    message="[DARIUS]:Which spell do we use to look the location we're at?\n";
    write(1, message, strlen(message));
    while(1){
        scanf("%s",answer);
        if(strcmp(answer, "pwd")==0){
            printf("Correct!\n");
            break;
        }else{
            printf("Incorrect! Try again!");
        }
    }
    message="[DARIUS]:Which spell do we use to see the contents of a directory? (just the contents)\n";
    write(1, message, strlen(message));
    while(1){
        scanf("%s",answer);
        if(strcmp(answer, "ls")==0){
            printf("Correct!\n");
            break;
        }else{
            printf("Incorrect! Try again!");
        }
    }
    message="[DARIUS]:Which spell do we use to move a file from a directory to another?\n";
    write(1, message, strlen(message));
    while(1){
        scanf("%s",answer);
        if(strcmp(answer, "mv")==0){
            printf("Correct!\n");
            break;
        }else{
            printf("Incorrect! Try again!");
        }
    }
    message="[DARIUS]:Which spell do we use to change directory?\n";
    write(1, message, strlen(message));
    while(1){
        scanf("%s",answer);
        if(strcmp(answer, "cd")==0){
            printf("Correct!\n");
            break;
        }else{
            printf("Incorrect! Try again!");
        }
    }
    message="[DARIUS]:Which spell do we use to see the information of the contents of a directory?\n";
    write(1, message, strlen(message));
    while(1){

        scanf(" %[^\n]%*c", answer);
            if(strcmp(answer, "ls -l")==0){
                printf("Correct!\n");
                break;
            }else{
                printf("Incorrect! Try again!");
            }


    }
    message="[DARIUS]:Which spell do we use to search a file's contents?\n";
    write(1, message, strlen(message));
    while(1){
        scanf("%s",answer);
        if(strcmp(answer, "grep")==0){
            printf("Correct!\n");
            break;
        }else{
            printf("Incorrect! Try again!");
        }
    }
    message="[DARIUS]: Which spell do we use to copy a file?";
    write(1, message, strlen(message));
    while(1){
        scanf("%s",answer);
        if(strcmp(answer, "cp")==0){
            printf("Correct!\n");
            break;
        }else{
            printf("Incorrect! Try again!");
        }
    }
    message="Well well! What do we have here! A great magician!\n";
    write(1, message, strlen(message));
    enterDetector();
    message="[DARIUS]: Well now we can go on! You now have to decide going to the forest or the desert entrance\n";
    message="[DARIUS]: From now on, I let you make your decisions :), but remember you only can use ls and cd when you are not fighting :), GOOD LUCK!\n";
}

int main ()
{
    getcwd(programPath, sizeof programPath);
    getcwd(actualPath, sizeof actualPath);
    sprintf(programPath, "%s/", programPath);
    stage=0;
    scene=0;
    tutorialStage();
    stage=1;
    scene=0;
    char *message="[SIMULATION SYSTEM]: SIMULATION END SEQUENCE INITIALIZED, PLEASE INSERT PASSWORD TO CONFIRM\n";
    write(1, message, strlen(message));
    char received[100];
    int r;
    struct termios saved_tm;
    while(1) {
        setTermNoCanon(&saved_tm);
        scanf("%s", received);
        recover_tm(&saved_tm);
        if(strcmp(received, "DARIUSFINALTEST")==0){

            printf("PASSWORD CONFIRMED\n");
            break;
        }
    }
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    dariusSpellTest();
    done = 1;
    stage=2;
    executeCommandPrompt();
}




