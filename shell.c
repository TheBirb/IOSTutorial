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
int stage;
int scene;
int done;
char actualPath[200];
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
                return -1;
            }else{
                chdir(argv[1]);
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
    write(0, m, strlen(m));
    while(getchar()!='\n'){
        ;
    }
    return 0;

}
int executeCommandPrompt(){
    char * Prompt = "myShell0> ";
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
                    default:
                        return 0;
                }
            }
            execute(argc, args);
        }
        if (eof) break;
        if(done==0) break;
    }
    return 0;
}
void tutorialStage(){
    char *warning1="TUTORIAL:Write the instruction correctly!\n"; //instruction is not the expected one
    char *warning2="TUTORIAL:Write the directory correctly!\n";  // directory is no the expected one
    char *warning3="TUTORIAL: You dont need to put anything else!\n"; //more arguments than expected
    char *question;
    char *answer;
    int i;
    //Intro
    char *message ="  ____          _      _                    _ \n";
    write(0,message, strlen(message));
    message=" / ___|___   __| | ___| |    __ _ _ __   __| |\n";
    write(0,message, strlen(message));
    message ="| |   / _ \\ / _` |/ _ \\ |   / _` | '_ \\ / _` |\n";
    write(0,message, strlen(message));
    message="| |__| (_) | (_| |  __/ |__| (_| | | | | (_| |\n";
    write(0,message, strlen(message));
    message=" \\____\\___/ \\__,_|\\___|_____\\__,_|_| |_|\\__,_|;\n";
    write(0,message, strlen(message));
    message="Welcome to codeland a place where you will learn some basic linux commands!\n";
    write(0,message, strlen(message));
    enterDetector();
    message="Firstly, let me introduce myself, I'm Darius, your Artificial Assistant, I will teach you the basic commands also know in our world\n";
    write(0, message, strlen(message));
    message ="as spells. Yes you have read correctly, in this world linux commands are used as spells to defeat monsters\n";
    write(0, message, strlen(message));
    enterDetector();
    question ="Is there something you want to know about this world?\n";
    write(0, question, strlen(question));
    message="TUTORIAL:To select a question you must write the number of the question and hit enter!\n";
    answer="1:How is this world called?\n";
    write(0, answer, strlen(answer));
    answer="2:I have no questions, let's continue\n";
    write(0, answer, strlen(answer));
    write(0, message, strlen(message));
    char select[1];
    read(1, select, sizeof select);
    if(strcmp(select,"1")==0){
        message="This world is called CodeLand!\n";
        write(0, message, strlen(message));
        message="I'm so excited for you being here! Let's go straight to the basics of wizardry\n";
        write(0, message, strlen(message));
    }else{
        message="As I see you don't wan't to know anything else, let's start with the basics\n";
        write(0, message, strlen(message));
    }
    //stage==0--------------------------------------------------------------------------------------
    //scene 0 ls
    scene=0;
    message="The first thing we need to do is locate the Tutorial zone\n";
    write(0, message, strlen(message));
    message="To do this we use a spell called 'ls' to see all the files that are in the same directory\nYou just need to type 'ls' to search the actual zone you are in!\n";
    write(0, message, strlen(message));
    enterDetector();
    message="Let's put it into practice!\n";
    write(0, message, strlen(message));
    enterDetector();

    i=4;
    done=1;
    while(i!=0){
        i= executeCommandPrompt();
        if(i==1){
            write(0, warning1, strlen(warning1));
        }else if(i==3){
            write(0, warning2, strlen(warning3));
        }
    }
    //scene 1   ls in another directory
    scene=1;
    message="Well done!, As you can see above we can see all the files in the same directory and we can also see the TutorialZone!\n";
    write(0, message, strlen(message));
    enterDetector();
    message="We can use this spell to search inside zones we are not in! What a powerful spell!!!!\n";
    write(0, message, strlen(message));
    enterDetector();
    message="Lets try it! Write 'ls TutorialZone' and watch what happens!\n";
    write(0, message, strlen(message));
    enterDetector();
    i=4;
    done=1;
    while(i!=0){
        i= executeCommandPrompt();
        if(i==1){
            write(0, warning1, strlen(warning1));
        }else if(i==2){
            write(0, warning2, strlen(warning2));
        }
    }
    message="As you can see, we now know about all the files that are in the same directory\n The '.' entry refers to the actual directory we are in and the '..' is the directory where the directory we are now on is\n It is also known as the 'parent' directory\n";
    write(0, message, strlen(message));
    enterDetector();
    //scene 2 ls -l
    scene=2;
    message="There is an alteration of the spell 'ls' that can give us a lot more information of the files\n";
    write(0, message, strlen(message));
    message="This spell is know as 'ls -l' and tells us information about a file or directory, like who is the owner of the file, the date the file was created, the size, etc.\n";
    write(0, message, strlen(message));
    message="Let's try it\n";
    write(0, message, strlen(message));
    enterDetector();
    i=4;
    done=1;
    while(i!=0){
        i= executeCommandPrompt();
        if(i==1){
            write(0, warning1, strlen(warning1));
        }else if(i==3){
            write(0, warning2, strlen(warning3));
        }
    }
    //scene 3 ls -l to another
    scene=3;
    message="So, as I said before, the 'ls -l' spell is an alteration, this means we can use it also to see what is inside other directories.\n";
    write(0, message, strlen(message));
    enterDetector();
    message="Let's do 'ls -l TutorialZone' and see what's the result!\n";
    write(0, message, strlen(message));
    enterDetector();
    i=4;
    done=1;
    while(i!=0){
        i= executeCommandPrompt();
        if(i==1){
            write(0, warning1, strlen(warning1));
        }else if(i==3){
            write(0, warning2, strlen(warning3));
        }
    }
    //scene 4 cd
    message="As I told you before, now we can see in detail the characteristics of the files in that directory!\n";
    write(1, message, strlen(message));
    enterDetector();
    scene=4;
    message="Now we need to travel to the TutorialZone!\n";
    write(1, message, strlen(message));
    enterDetector();
    message="To do this, write on the command prompt cd and put the directory or folder you want to switch to\n";
    write(1, message, strlen(message));
    enterDetector();
    message="Remember to put an space between the cd and the directory!!\n";
    write(1, message, strlen(message));
    enterDetector();
    i=4;
    done=1;
    while(i!=0){
        i= executeCommandPrompt();
        if(i==1){
            write(1, warning1, strlen(warning1));
        }else if(i==2){
            write(1, warning2, strlen(warning2));
        }
    }
    //scene 5 pwd
    scene=5;
    message="Nicely done! Now you are in the tutorial zone\n";
    write(1, message, strlen(message));
    enterDetector();
    message="And you might think, how does Darius know we are already in the tutorialZone?\n";
    write(1, message, strlen(message));
    enterDetector();
    message="Well, there is a spell know as 'pwd' that tells us where we are\n";
    write(1, message, strlen(message));
    enterDetector();
    message="Let's try it!\n";
    write(1, message, strlen(message));
    done=1;
    enterDetector();
    i=4;
    while(i!=0){
        i= executeCommandPrompt();
        if(i==1){
            write(1, warning1, strlen(warning1));
        }else if(i==2){
            write(1, warning2, strlen(warning2));
        }else if(i==3){
            write(1, warning3, strlen(warning3));
        }
    }
    //scene 6 cat
    scene=6;
    message="The last string after the last '/' is where you are currently in, and the others at the left are their parents\n";
    write(1, message, strlen(message));
    enterDetector();
    message="In this tutorial place we have a level one Virus that I have already weakened\n";
    write(1, message, strlen(message));
    enterDetector();
    message="To see the contents of this virus and to see that I have already left my mark in this virus we can use the spell grep\n";
    write(1, message, strlen(message));
    enterDetector();
    message="To do this there is a spell called 'cat', we can use it to see al the contents of a file\n";
    write(1, message, strlen(message));
    enterDetector();
    message="Let's use it on the Virus! Write 'cat VirusLv1'!\n";
    write(1, message, strlen(message));
    enterDetector();
    i=4;
    done=1;
    while(i!=0){
        i= executeCommandPrompt();
        if(i==1){
            write(1, warning1, strlen(warning1));
        }else if(i==2){
            write(1, warning2, strlen(warning2));
        }else if(i==3){
            write(1, warning3, strlen(warning3));
        }
    }
    //scene 7 grep
    scene=7;
    message="Well! As you can see it is pretty difficult to search for the marks I left!\n";
    write(1, message, strlen(message));
    enterDetector();
    message="Oh! Don't worry! The wizards of these lands thought of a better way to search for things!\n There is a spell called 'grep' used to do this";
    write(1, message, strlen(message));
    enterDetector();
    message="Just write 'grep Darius VirusLv1' and see the magic!\n";
    write(1, message, strlen(message));
    enterDetector();
    i=4;
    done=1;
    while(i!=0){
        i= executeCommandPrompt();
        if(i==1){
            write(1, warning1, strlen(warning1));
        }else if(i==2){
            write(1, warning2, strlen(warning2));
        }else if(i==3){
            write(1, warning3, strlen(warning3));
        }
    }
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
}




