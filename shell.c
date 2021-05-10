// CodeLand, configured, designed and adapted by Jon Moríñigo
//Level 0 and 1 designed by Jon Moríñigo
//Level 2 designed by Jon Da Silva
//Piping, level management and instruction administration designed by Jon Moriñigo

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
    write(1, m, strlen(m));
    while(getchar()!='\n'){}
    return 0;

}
int executeCommandPrompt(){
    char *Prompt="SpellBook$ ";
    int eof= 0;
    int argc;
    char *args[MAXARGS];
    while (1) {
        write(1, Prompt, strlen(Prompt));
        if (read_args(&argc, args, MAXARGS, &eof) && argc > 0) {
            if (args >=0) {
                if (stage == 0) { //tutorial
                    switch (scene) {
                        case 0:
                            if (strcmp(args[0], "ls") != 0) {
                                return 1;
                            } else if (argc > 1) {
                                return 3;
                            } else {
                                done = 0;
                            }
                            break;
                        case 1:
                            if (argc < 2) {
                                printf("Write well the instruction!\n");
                                return 4;
                            } else if (strcmp(args[0], "ls") != 0) {
                                return 1;
                            } else if (strcmp(args[1], "TutorialZone")) {
                                return 2;
                            } else if (argc > 2) {
                                return 3;
                            } else {
                                done = 0;
                            }
                            break;
                        case 2:
                            if (argc < 2) {
                                printf("Write well the instruction!\n");
                                return 4;
                            } else if (strcmp(args[0], "ls") != 0) {
                                return 1;
                            } else if (strcmp(args[1], "-l")) {
                                return 1;
                            } else if (argc > 2) {
                                return 3;
                            } else {
                                done = 0;
                            }
                            break;
                        case 3:
                            if (argc < 3) {
                                printf("Write well the instruction!\n");
                                return 4;
                            } else if (strcmp(args[0], "ls") != 0) {
                                return 1;
                            } else if (strcmp(args[1], "-l")) {
                                return 1;
                            } else if (strcmp(args[2], "TutorialZone")) {
                                return 2;
                            } else if (argc > 3) {
                                return 3;
                            } else {
                                done = 0;
                            }
                            break;
                        case 4:
                            if (argc < 2) {
                                printf("Write well the instruction!\n");
                                return 4;
                            } else if (strcmp(args[0], "cd") != 0) {
                                return 1;
                            } else if (strcmp(args[1], "TutorialZone")) {
                                return 2;
                            } else {
                                done = 0;
                            }
                            break;
                        case 5:
                            if (strcmp(args[0], "pwd") != 0) {
                                return 1;
                            } else if (argc > 1) {
                                return 3;
                            } else {
                                done = 0;
                            }
                            break;
                        case 6:
                            if (argc < 2) {
                                printf("Write well the spell!\n");
                                return 4;
                            } else if (strcmp(args[0], "cat") != 0) {
                                return 1;
                            } else if (strcmp(args[1], "VirusLv1") != 0) {
                                return 2;
                            } else if (argc > 2) {
                                return 3;
                            } else {
                                done = 0;
                            }
                            break;
                        case 7:
                            if (argc < 3) {
                                printf("Write well the instruction!\n");
                                return 4;
                            } else if (strcmp(args[0], "grep") != 0) {
                                return 1;
                            } else if (strcmp(args[1], "Darius") != 0) {
                                return 2;
                            } else if (strcmp(args[2], "VirusLv1") != 0) {
                                return 2;
                            } else if (argc > 3) {
                                return 3;
                            } else {
                                done = 0;
                            }
                            break;
                        case 8:
                            if (argc < 2) {
                                printf("Write well the instruction!\n");
                                return 4;
                            } else if (strcmp(args[0], "touch") != 0) {
                                return 1;
                            } else if (argc > 2) {
                                return 3;
                            } else {
                                done = 0;
                            }
                            break;
                        case 9:
                            if (argc < 1) {
                                printf("Write well the instruction!\n");
                                return 4;
                            } else if (strcmp(args[0], "ls") != 0) {
                                return 1;
                            } else if (argc > 1) {
                                return 3;
                            } else {
                                done = 0;
                            }
                            break;
                        case 10:
                            if (argc < 3) {
                                printf("Write well the instruction!\n");
                                return 4;
                            } else if (strcmp(args[0], "mv") != 0) {
                                return 1;
                            } else if (strcmp(args[1], "fireball") != 0) {
                                return 2;
                            } else if (strcmp(args[2], "Trash") != 0) {
                                return 2;
                            } else if (argc > 3) {
                                return 3;
                            } else {
                                done = 0;
                            }
                            break;
                        case 11:
                            if (argc < 3) {
                                printf("Write well the instruction!\n");
                                return 4;
                            } else if (strcmp(args[0], "cp") != 0) {
                                return 1;
                            } else if (strcmp(args[1], "fireball") != 0) {
                                return 2;
                            } else if (strcmp(args[2], "fireball2") != 0) {
                                return 2;
                            } else if (argc > 3) {
                                return 3;
                            } else {
                                done = 0;
                            }
                            break;
                        default:;
                    }
                } else if (stage == 3) {

                    if (strcmp(args[0], "cd") == 0 || strcmp(args[0], "ls") == 0 || strcmp(args[0], "cat") == 0 || strcmp(args[0], "pwd")==0) {
                        if (strcmp(args[0], "cd") == 0) {
                            char dir[200];
                            getcwd(dir, 200);
                            char actualdir[200];
                            char dirlvl2[200];
                            sprintf(dir, "%s/%s", dir, args[1]);
                            sprintf(actualdir, "%s%s", programPath,
                                    "TutorialZone/DariusSpellTest/DesertEntrance/Desert");
                            sprintf(dirlvl2, "%s%s", programPath,
                                    "TutorialZone/DariusSpellTest/Forest/MaslovenyCastle");

                            if (strcmp(actualdir, dir) == 0) {
                                chdir("Desert");

                                return 4;
                            }else if(strcmp(dirlvl2,dir)==0){
                                chdir("MaslovenyCastle");
                                return 5;
                            }
                        }
                    } else {
                        printf("You can't use that spell!\n");
                        return 0;
                    }
                } else if (stage == 4) {
                        if (argc < 5) {
                            printf("%d\n", argc);
                            printf("Write well the instruction!\n");
                            return 4;
                        } else if (strcmp(args[0], "ls") != 0) {
                            return 1;
                        } else if (strcmp(args[1], "-l") != 0) {
                            return 2;
                        } else if (strcmp(args[2], "|") != 0) {
                            return 1;
                        } else if (strcmp(args[3], "grep") != 0) {
                            return 1;
                        } else if (strcmp(args[4], "book") != 0) {
                            return 2;
                        } else if (argc > 5) {
                            return 3;
                        } else {
                            done = 0;
                        }



                } else if(stage==5) {
                    switch(scene) {
                        case 0: //Start
                            if (strcmp(args[0], "cat") != 0) {
                                execute(argc, args);
                                return 4;
                            } else if (strcmp(args[1], "varonesa") != 0) {
                                return 4;
                            } else if (argc < 1) {
                                return 3;
                            } else {
                                done = 0;
                            }
                            break;
                        case 1: // learning your spells
                            if (strcmp(args[0], "cat") != 0) {
                                execute(argc, args);
                                return 4;
                            } else if (strcmp(args[1], "info") != 0) {
                                return 4;
                            } else if (argc < 1) {
                                return 3;
                            } else {
                                done = 0;
                            }
                            break;
                        case 2: //learning attack
                            if (strcmp(args[0], "cat") != 0) {
                                execute(argc, args);
                                return 4;
                            } else if (strcmp(args[1], "shoot") != 0) {
                                return 4;
                            } else if (argc < 1) {
                                return 3;
                            } else {
                                done = 0;
                            }
                            break;
                        case 3: // battle
                            if (strcmp(args[0], "cat") != 0) {
                                execute(argc, args);
                                return 4;
                            }else if (strcmp(args[1], "shoot") != 0) {
                                return 2;
                            }else if(argc<1){
                                return 3;
                            }else{
                                done=0;
                            }
                            break;
                        default:
                            return 0;
                    }
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

            }
        }
        if (eof) break;
        if (done == 0) break;
    }

    return 0;
}


//password utils
    void setTermNoCanon(struct termios *savedtm) {
        struct termios tm;
        tcgetattr(0, &tm);
        tcgetattr(0, savedtm);
        tm.c_lflag &= ~(ICANON | ECHO);
        tm.c_cc[VMIN] = 1;
        tm.c_cc[VTIME] = 0;
        tcsetattr(0, TCSANOW, &tm);
    }
    void recover_tm(struct termios *tm) {
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
        message = "[DARIUS]: I'm so excited for you being here! Let's go straight to the basics of wizardry\n";
        write(1, message, strlen(message));
        enterDetector();

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
        message = "[DARIUS]: Now if you do ls you will see the new file was created!\n";
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
        scene = 11;
        message = "[DARIUS]: OK, now we are in the Trash. Yes literally, but try 'cp fireball fireball2\n";
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
        scene = 0;
        message = "[DARIUS]: Now do ls :)\n";
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
        message = "[DARIUS]: and MAGIC!!!! Now we have to copies of fireball";
        write(1, message, strlen(message));
        enterDetector();
        scene = 10;
        message = "[DARIUS]: But right now we don't want that there, so let's throw it to the trash to do this we have a spell to move things!\n";
        write(1, message, strlen(message));
        enterDetector();
        message = "[DARIUS]: Yes! You've read correctly! Use the spell 'mv' to move fireball to the trash!\n"
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
        message = "[DARIUS]: Now the file is in the trash and it will eventually disappear!\n";
        write(1, message, strlen(message));
        enterDetector();
        chdir("Trash");
        system("rm fireball");
        chdir("..");
        system("rm fireball2");
        scene = 12;
        message = "[DARIUS]: And a little tip! You can use man [command] or help [command] to see or remember how commands work ;)\n ";
        write(1, message, strlen(message));
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        message = "[DARIUS]: WOW! You've come so far! But I have some bad news!\n"
                  "[DARIUS]: I've got a lot of student's they a lot of them didn't deserve to enter the real CodeLand\n"
                  "[DARIUS]: Yes! All you you have experienced was a simulation of CodeLand\n"
                  "[DARIUS]: Now, YOU have to prove me you have learned >:)\n";
        write(1, message, strlen(message));
        message = "[DARIUS]: The password for the test is 'DARIUSFINALTEST'\n";
        write(1, message, strlen(message));
        enterDetector();

    }
    void dariusSpellTest() {
        chdir("DariusSpellTest");
        char *message = "[DARIUS]: WELCOME!!!!!\n";
        write(1, message, strlen(message));
        message = "[DARIUS]: Let's start directly with the test!\n";
        write(1, message, strlen(message));
        char answer[100];
        message = "[DARIUS]:Which spell do we use to see the contents of a file?\n";
        write(1, message, strlen(message));
        while (1) {
            scanf("%s", answer);
            if (strcmp(answer, "cat") == 0) {
                printf("Correct!\n");
                break;
            } else {
                printf("Incorrect! Try again!");
            }
        }
        message = "[DARIUS]:Which spell do we use to look the location we're at?\n";
        write(1, message, strlen(message));
        while (1) {
            scanf("%s", answer);
            if (strcmp(answer, "pwd") == 0) {
                printf("Correct!\n");
                break;
            } else {
                printf("Incorrect! Try again!");
            }
        }
        message = "[DARIUS]:Which spell do we use to see the contents of a directory? (just the contents)\n";
        write(1, message, strlen(message));
        while (1) {
            scanf("%s", answer);
            if (strcmp(answer, "ls") == 0) {
                printf("Correct!\n");
                break;
            } else {
                printf("Incorrect! Try again!");
            }
        }
        message = "[DARIUS]:Which spell do we use to move a file from a directory to another?\n";
        write(1, message, strlen(message));
        while (1) {
            scanf("%s", answer);
            if (strcmp(answer, "mv") == 0) {
                printf("Correct!\n");
                break;
            } else {
                printf("Incorrect! Try again!");
            }
        }
        message = "[DARIUS]:Which spell do we use to change directory?\n";
        write(1, message, strlen(message));
        while (1) {
            scanf("%s", answer);
            if (strcmp(answer, "cd") == 0) {
                printf("Correct!\n");
                break;
            } else {
                printf("Incorrect! Try again!");
            }
        }
        message = "[DARIUS]:Which spell do we use to see the information of the contents of a directory?\n";
        write(1, message, strlen(message));
        while (1) {

            scanf(" %[^\n]%*c", answer);
            if (strcmp(answer, "ls -l") == 0) {
                printf("Correct!\n");
                break;
            } else {
                printf("Incorrect! Try again!");
            }


        }
        message = "[DARIUS]:Which spell do we use to search a file's contents?\n";
        write(1, message, strlen(message));
        while (1) {
            scanf("%s", answer);
            if (strcmp(answer, "grep") == 0) {
                printf("Correct!\n");
                break;
            } else {
                printf("Incorrect! Try again!");
            }
        }
        message = "[DARIUS]: Which spell do we use to copy a file?";
        write(1, message, strlen(message));
        while (1) {
            scanf("%s", answer);
            if (strcmp(answer, "cp") == 0) {
                printf("Correct!\n");
                break;
            } else {
                printf("Incorrect! Try again!");
            }
        }
        message = "Well well! What do we have here! A great magician!\n";
        write(1, message, strlen(message));
        enterDetector();
        message = "[DARIUS]: Well now we can go on! You now have to decide going to the forest or the desert entrance\n";
        write(1, message, strlen(message));
        enterDetector();
        message = "[DARIUS]: From now on, I let you make your decisions :), but remember you only can use ls,cd,pwd and cat when you are not fighting :), GOOD LUCK!\n";
        write(1, message, strlen(message));
        enterDetector();

    }
    int freewill() {
        done = 1;
        int x=0;
        while(x==0){
            x= executeCommandPrompt();

        }
        return x;

    }
    int desert(){
        stage=4;
        char *warning1 = "TUTORIAL:Write the instruction correctly!\n"; //instruction is not the expected one
        char *warning2 = "TUTORIAL:Write the directory correctly!\n";  // directory is no the expected one
        char *warning3 = "TUTORIAL: You dont need to put anything else!\n"; //more arguments than expected
        char *message="[OLD PIPE MAN]: Oh! Hello traveler! It's been a while since I've seen another human!\n";
        write(1, message, strlen(message));
        enterDetector();
        message="[OLD PIPE MAN]: Excuse me sir, but I need some help to find some things I left in the sand and with all these\n"
                "sand storms all the things have mixed!\n";
        write(1, message, strlen(message));
        enterDetector();
        message="[OLD PIPE MAN]: Oh! Excuse my manners! I should have introduced myself! My name is MasterPipe! When I was young I was one of\n"
                "the greatest wizards of CodeLand! But during my research I found black magic and I went crazy!\n";
        write(1, message, strlen(message));
        enterDetector();
        message="[OLD PIPE MAN]: Don't worry! I've learned after so many years how to control this knowledge!\n";
        write(1, message, strlen(message));
        enterDetector();
        message="[OLD PIPE MAN]: I'm going to taught you in return how to use this black magic!\n"
                "                You will need it to search for my things! I'm so old I barely have the strength to\n"
                "                execute those spells!\n";
        write(1, message, strlen(message));
        enterDetector();
        message="[OLD PIPE MAN]: Well, let's get into it! The first thing you need to know is that this technique is called Piping!\n"
                "                It is used to concatenate spells\n";
        write(1, message, strlen(message));
        enterDetector();
        message="[OLD PIPE MAN]: Today we are just going to use ls to see all the things I have here, and grep to filter those things and find what I want\n";
        write(1, message, strlen(message));
        enterDetector();
        message="[OLD PIPE MAN]: Put 'ls -l | grep book' to see the magic! Oh I didn't tell you '|' is used to do piping, is obtained pressing altgr+1\n";
        write(1, message, strlen(message));
        enterDetector();
        done = 1;
        scene =1;
        int i = 4;
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
        message="[OLD PIPE MAN]: Wow! You are true wizard!!!!\n";
        printf("%s", message);
        message="[OLD PIPE MAN]: OH!!! THANK YOU SO MUCH!! Now I'm leaving I must get back to my research!\n";
        printf("%s", message);
        message="Suddenly the old man disappears and you are left alone!\n";
        printf("%s", message);

        scene=3;
        return 0;
    }
    void MaslovenyCastle(){
    char *warning1="LEVEL 2: Write the instruction correctly!\n"; //instruction is not the expected one
    char *warning2="LEVEL 2: Write the directory correctly!\n";  // directory is no the expected one
    char *question;
    char *answer;
    int i;
    int trys;
    lives = 5;
    //Intro

    char *message = " _     _______     _______ _       ____  \n"
                    "| |   | ____\\ \\   / / ____| |     |___ \\ \n"
                    "| |   |  _|  \\ \\ / /|  _| | |       __) |\n"
                    "| |___| |___  \\ V / | |___| |___   / __/ \n"
                    "|_____|_____|  \\_/  |_____|_____| |_____|\n"
                    "                                         \n";
    printf("%s", message);
    message="[DARIUS]: Welcome to 2nd level of Codeland!\n";
    printf("%s", message);
    enterDetector();
    message="[DARIUS]: Here you will fight against a boss that lives on Codeland.\n";
    printf("%s", message);
    enterDetector();
    question ="[DARIUS]: Are you ready for this?\n";
    printf("%s", question);
    message="[DARIUS]: Type 1 or 2 to answer the question\n";
    answer="1:Yes\n";
    printf("%s", answer);
    answer="2:No\n";
    printf("%s", answer);
    printf("%s", message);
    char select[1];
    read(0, select, sizeof select);
    enterDetector();
    if(strcmp(select,"1")==0){
        message="[DARIUS]: Be ready for fight against the most powerfull lady of Codeland!\n";
        printf("%s", message);
    }else{
        message="[DARIUS]: I thought you were more braver...\n";
        printf("%s", message);
    }
    //stage==2--------------------------------------------------------------------------------------
    //scene 0 Start
    scene=0;
    message="[DARIUS]: Let's begin.\n";
    printf("%s", message);
    message="[DARIUS]: You have 5 lives, each live will be show like hearts (♥) \n";
    printf("%s", message);
    message="[DARIUS]: First off all you have to know a little bit about the lady.\nUse your knowledge about bash commands for explore directories and files.\n";
    printf("%s", message);
    enterDetector();
    i=4;
    done=1;
    trys = 0;
    while(i!=0) {
        updateLives();
        i = executeCommandPrompt();
        trys = trys + 1;
        if (trys > 3) {
            message="TIP: Use 'cd' command for explore directories and 'cat' command for read files. If you are lost can use the map ('pwd' command) for know where you are.\n";
            trys = 0;
            printf("%s", message);
        }else{
            if (i == 1) {
                printf("%s", warning1);
            } else if (i == 3) {
                printf("%s", warning2);
            }
        }
    }
    enterDetector();
    //scene 1   learning your spells
    scene=1;
    message="[DARIUS]: Well done! Now we know something about this rare lady...\nNow you have to learn about your spells. Keep exploring directories.\n";
    printf("%s", message);
    enterDetector();
    i=4;
    done=1;
    trys = 0;
    while(i!=0) {
        updateLives();
        i = executeCommandPrompt();
        trys = trys + 1;
        if (trys > 3) {
            message="TIP: Use 'cd' command for explore directories and 'cat' command for read files. If you are lost can use the map ('pwd' command) for know where you are.\n";
            trys = 0;
            printf("%s", message);
        }else{
            if (i == 1) {
                printf("%s", warning1);
            } else if (i == 3) {
                printf("%s", warning2);
            }
        }
    }
    enterDetector();
    //scene 2 learning attack
    scene=2;
    message="[DARIUS]: Well done! Now you will learn how to attack. Keep exploring your spells directory\n";
    printf("%s", message);
    enterDetector();
    i=4;
    done=1;
    trys = 0;
    while(i!=0) {
        updateLives();
        i = executeCommandPrompt();
        trys = trys + 1;
        if (trys > 3) {
            message="TIP: Use 'cd' command for explore directories and 'cat' command for read files. If you are lost can use the map ('pwd' command) for know where you are.\n";
            trys = 0;
            printf("%s", message);
        }else{
            if (i == 1) {
                printf("%s", warning1);
            } else if (i == 3) {
                printf("%s", warning2);
            }
        }
    }
    enterDetector();
    //scene 3 battle
    scene=3;
    message="[DARIUS]: Now you are ready for the battle! Let's battle began\n";
    printf("%s", message);
    message="[DARIUS]: For kill Varonesa you will have to do the correct spells sequence\n";
    printf("%s", message);
    enterDetector();
    i=4;
    done=1;
    trys = 0;
    int combo = 0;
    while(combo<3) {
        updateLives();
        i = executeCommandPrompt();
        trys = trys + 1;
        if (trys > 3) {
            message="TIP: Use 'cd' command for explore directories and 'cat' command for read files. If you are lost can use the map ('pwd' command) for know where you are.\n";
            trys = 0;
            printf("%s", message);
        }else{
            if (i == 1) {
                printf("%s", warning1);
            } else if (i == 3) {
                printf("%s", warning2);
            }
        }
        char cwd[200];
        if(i==0){
            if(combo==0){
                if (strcmp(getcwd(cwd, sizeof(cwd)), "/home/jon/CLionProjects/IOSproyect/TutorialZone/DariusSpellTest/Forest/MaslovenyCastle/myspells/fireball") == 0 ){
                    message="[DARIUS]: HOHOHOHO! That had to hurt!\n";
                    printf("%s", message);
                    combo=1;
                }else{
                    //quitar vida
                    message="[DARIUS]: Maybe that isn't the correct first spell...\n";
                    printf("%s", message);
                    lives--;
                }
            }else if (combo==1){
                if (strcmp(getcwd(cwd, sizeof(cwd)), "/home/jon/CLionProjects/IOSproyect/TutorialZone/DariusSpellTest/Forest/MaslovenyCastle/myspells/frostbolt") == 0 ){
                    message="[DARIUS]: Oooh nice! I can feel how cold it must be.\n";
                    printf("%s", message);
                    combo=2;
                }else{
                    //quitar vida
                    message="[DARIUS]: She is hot from fireball maybe you have to calm...\n";
                    printf("%s", message);
                    lives--;

                }
            }else if (combo==2){
                if (strcmp(getcwd(cwd, sizeof(cwd)), "/home/jon/CLionProjects/IOSproyect/TutorialZone/DariusSpellTest/Forest/MaslovenyCastle/myspells/electric_Shock") == 0 ){
                    message="[DARIUS]: Well! Look how it moves, it looks like a fish hahahahhaha!\n";
                    printf("%s", message);
                    combo=3;
                }else{
                    //quitar vida
                    message="[DARIUS]: Nah this is not how it ends, let's try play with chemical reactions\n";
                    printf("%s", message);
                    lives--;
                }
            }
        }
    }
    message="[DARIUS]: Nice! You killed Varonesa! Great job!\n";
    printf("%s", message);
    enterDetector();
}
void updateLives(){
    int i = 0;
    while (i<5){
        printf("(♥)");
        i++;
    }
    printf("\n");
}
    int main() {
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
        stage = 3;
        scene = 0;
        int where = 0;
        int finish = 0;
        int deserti=0;
        while(1){
            where = freewill();
            if(where==4){
                if(deserti==0){
                    desert();
                    deserti=1;
                    stage=3;
                    where=0;
                }

            }else if(where==5){
                //level2();
            }else if(where==6){
                finish=1;
                break;
            }
        }



    return 0;

    }




