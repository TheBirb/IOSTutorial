#include<stdio.h>
#include<unistd.h>
int main(int argc, char **argv){

    if(strcmp(argv[1], "cd")==0){
        printf("NAME\n"
               "    cd - Change the shell working directory.\n"
               "\n"
               "SYNOPSIS\n"
               "    cd [dir]\n"
               "\n"
               "DESCRIPTION\n"
               "    Change the shell working directory.\n"
               "    \n"
               "AUTHORS\n"
               "     Written by Jon Moríñigo, Jon Da Silva\n");

    }else if(strcmp(argv[1], "ls")==0){
                printf("LS(1)                         User Commands                         LS(1)\n"
                       "\n"
                       "NAME\n"
                       "       ls - list directory contents\n"
                       "\n"
                       "SYNOPSIS\n"
                       "       ls [OPTION]... [FILE]...\n"
                       "\n"
                       "DESCRIPTION\n"
                       "       List  information  about  the  FILEs  (the  current  directory  by\n"
                       "       default).  Sort entries alphabetically.\n"
                       "\n"
                       "       -l     use a long listing format\n"
                       "\n"
                       "AUTHORS\n"
                       "    Written by Jon Da Silva, Jon Moríñigo.\n"
                       "\n");
    }else if(strcmp(argv[1], "pwd")==0){
        printf("NAME\n"
               "    pwd - Print the name of the current working directory.\n"
               "\n"
               "SYNOPSIS\n"
               "    pwd [-LP]\n"
               "\n"
               "DESCRIPTION\n"
               "    Print the name of the current working directory.\n"
               "    \n"
               "AUTORS\n"
               "     Written by Jon Da Silva.   \n");
    }else if(strcmp(argv[1], "cat")==0){
        printf("CAT(1)                        User Commands                        CAT(1)\n"
               "\n"
               "NAME\n"
               "       cat - concatenate files and print on the standard output\n"
               "\n"
               "SYNOPSIS\n"
               "       cat [OPTION]... [FILE]...\n"
               "\n"
               "DESCRIPTION\n"
               "       cat    Copy standard input to standard output.\n"
               "\n"
               "AUTHORS\n"
               "     Written by Jon Moríñigo.      \n");

    }else if(strcmp(argv[1], "touch")==0){
        printf("TOUCH(1)                      User Commands                      TOUCH(1)\n"
               "\n"
               "NAME\n"
               "       touch - creates a file\n"
               "\n"
               "SYNOPSIS\n"
               "       touch FILE...\n"
               "\n"
               "DESCRIPTION\n"
               "       A FILE argument that does not exist is created empty\n"
               "AUTHORS\n"
               "       Written by Jon Moríñigo.\n"
               "\n");

    }else if(strcmp(argv[1], "mv")==0){
    printf("MV(1)                         User Commands                         MV(1)\n"
           "\n"
           "NAME\n"
           "       mv - move (rename) files\n"
           "\n"
           "SYNOPSIS\n"
           "       mv SOURCE DEST\n"
           "       mv SOURCE... DIRECTORY\n"
           "\n"
           "DESCRIPTION\n"
           "       Rename SOURCE to DEST, or move SOURCE(s) to DIRECTORY.\n"
           "\n"
           "AUTHORS\n"
           "       Written by Jorge Goicoechea\n"
           "\n");

    }else if(strcmp(argv[1], "cp")==0){

        printf("CP(1)                         User Commands                         CP(1)\n"
               "\n"
               "NAME\n"
               "       cp - copy files and directories\n"
               "\n"
               "SYNOPSIS\n"
               "       cp SOURCE... DEST\n"
               "\n"
               "DESCRIPTION\n"
               "       Copy SOURCE to DEST.\n"
               "\n"
               "AUTHORS\n"
               "       Written by Jon Da Silva.\n");

    }else if(strcmp(argv[1], "grep")==0){

        printf("GREP(1)                       User Commands                       GREP(1)\n"
               "\n"
               "NAME\n"
               "       grep - print lines that match patterns\n"
               "\n"
               "SYNOPSIS\n"
               "       grep PATTERN [FILE...]\n"
               "\n"
               "DESCRIPTION\n"
               "       grep  searches  for  PATTERN  in  each  FILE and prints each\n"
               "       line that matches the pattern.\n"
               "\n"
               "AUTHORS\n"
               "     Written by Jon Moríñigo.\n");
    }else if(strcmp(argv[1], "man")==0){

        printf("MAN(1)                      Manual pager utils                     MAN(1)\n"
               "\n"
               "NAME\n"
               "       man - an interface to the system reference manuals\n"
               "\n"
               "SYNOPSIS\n"
               "       man [man options]n"
               "\n"
               "DESCRIPTION\n"
               "       man is the system's manual pager.  Each page argument given to man\n"
               "       is normally the name of a function.  The  manual page associated\n"
               "       with each of these arguments is then found and displayed.\n"
               "\n"
               "       Conventional section names include NAME, SYNOPSIS,\n"
               "       DESCRIPTION and AUTHORS\n"
               "\n"
               "AUTHORS\n"
               "     Written by Jon Da Silva, Alexandra Aleina Pelipian");
    }else if(strcmp(argv[1], "help")==0)
        printf("NAME\n"
               "    help - Display information about builtin commands.\n"
               "\n"
               "SYNOPSIS\n"
               "    help [pattern ...]\n"
               "\n"
               "DESCRIPTION\n"
               "    Display information about builtin commands.\n"
               "    \n"
               "    Arguments:\n"
               "      PATTERN\tPattern specifying a help topic\n"
               "\n"
               "AUTHORS\n"
               "    Written by Alexandra Aleina Pelipian\n");
}
