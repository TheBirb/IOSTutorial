#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

    if(strcmp(argv[1],"pwd")==0){
        printf("pwd: pwd [-LP]\n");
        printf("Print the name of the current working directory.\n");
        printf("\nExit status:\nReturns 0 unless an invalid option is given or the current directory cannot be read.");
        goto END;
    }
    else if(strcmp(argv[1],"cd")==0){
        printf("cd: cd [dir]\n");
        printf("Change the shell working directory.\n");
        printf("\nChange the current directory to DIR.  The default DIR is the value of the HOME shell variable.\n");
        printf("\nThe variable CDPATH defines the search path for the directory containing DIR.");
        printf("Alternative directory names in CDPATH are separated by a colon (:).");
        printf("A null directory name is the same as the current directory.  If DIR begins");
        printf("with a slash (/), then CDPATH is not used.\n");
        printf("\nIf the directory is not found, and the shell option `cdable_vars' is set,");
        printf("the word is assumed to be  a variable name.  If that variable has a value, its value is used for DIR.\n");
        printf("\nExit status:\n");
        printf("Returns 0 if the directory is changed, and if $PWD is set successfully when -P is used; non-zero otherwise.\n");
        goto END;
    }
    else if(strcmp(argv[1],"ls")==0){
        printf("These options determine which files 'ls' lists information for.");
        printf("\nBy default, 'ls' lists files and the contents of any directories on the");
        printf("\ncommand line, except that in directories it ignores files whose names start with '.'.\n");
        printf("\n'-l'\n");
        printf("In addition to the name of each file, print the file type, file");
        printf("\nmode bits, number of hard links, owner name, group name, size, and");
        printf("\ntimestamp (*note Formatting file timestamps::), normally the");
        printf("\nmodification time.  Print question marks for information that cannot be determined.\n");
        goto END;
    }
    else if(strcmp(argv[1],"cat")==0){
        printf("'cat' copies each FILE ('-' means standard input), or standard input if none are given, to standard output.\n");
        goto END;
    }
    else if(strcmp(argv[1], "mv")==0){
        printf("'mv' moves or renames files (or directories).  Synopses:\n\n");
        printf("mv [OPTION]... [-T] SOURCE DEST\n mv [OPTION]... SOURCE... DIRECTORY\n mv [OPTION]... -t DIRECTORY SOURCE...\n");
        printf("\n* If two file names are given, 'mv' moves the first file to the second.\n");
        printf("\n* If the '--target-directory' ('-t') option is given, or failing that");
        printf("if the last file is a directory and the '--no-target-directory'");
        printf("('-T') option is not given, 'mv' moves each SOURCE file to the");
        printf("specified directory, using the SOURCEs' names.\n");
        goto END;
    }
    else if(strcmp(argv[1], "touch")==0){
        printf("touch [OPTION]... FILE...\n\n");
        printf("Any FILE argument that does not exist is created empty.\n");
        printf("\nA FILE argument string of '-' is handled specially and causes 'touch'\n to change the times of the file associated with standard output.\n");
        printf("By default, 'touch' sets file timestamps to the current time.\nBecause 'touch' acts on its operands left to right, the resulting\ntimestamps of earlier and later operands may disagree.  Also, the\ndetermination of what time is \"current\" depends on the platform.\nPlatforms with network file systems often use different clocks for the\n operating system and for file systems; because 'touch' typically uses\n file systems' clocks by default, clock skew can cause the resulting file\n timestamps to appear to be in a program's \"future\" or \"past\".\n");
        goto END;
    }
    else if(strcmp(argv[1],"cp")==0){
        printf("'cp' copies files (or, optionally, directories).  The copy is completely independent of the original.  You can either copy one file to another, or copy arbitrarily many files to a destination directory.  Synopses:");
        printf("\ncp [OPTION]... [-T] SOURCE DEST\n cp [OPTION]... SOURCE... DIRECTORY\n cp [OPTION]... -t DIRECTORY SOURCE...\n");
        printf("* If two file names are given, 'cp' copies the first file to the\n second.");
        printf("* If the '--target-directory' ('-t') option is given, or failing that\n if the last file is a directory and the '--no-target-directory'\n ('-T') option is not given, 'cp' copies each SOURCE file to the\n specified directory, using the SOURCEs' names.\n");
        printf("Generally, files are written just as they are read.  For exceptions,\n see the '--sparse' option below.");
        printf("By default, 'cp' does not copy .directories.  However, the '-R', '-a',\n and '-r' options cause 'cp' to copy recursively by descending into\n source directories and copying files to corresponding destination\n directories.");
        goto END;
    }
    else if(strcmp(argv[1], "grep")==0){
        printf("'grep' prints lines that contain a match for a pattern.\n\n");
        printf("The grep command consists of three parts in its most basic form. The first part starts with grep, followed by the pattern that you are searching for. \nAfter the string comes the file name that the grep searches through.");
        goto END;
    }
    else if(strcmp(argv[1], "man")==0){
        printf("'man' is the system's manual pager. Each page argument given to man is\n");
        printf("normally the name of a program, utility or function. The mangual page\n");
        printf("associated with each of these arguments is then found and displayed. A\n");
        printf("section, if provided, will direct man to look only in that section of\n");
        printf("the manual. The default action is to search in all of the available\n");
        printf("sections, following a pre-defined order and to show only the first page\n");
        printf("found, even if page exists in several sections.\n\n");
        printf("The table below shows the section numbers of the manual followed by the\n");
        printf("types of pages they contain.\n\n");
        printf("1   Executable programs or shell commands\n");
        printf("2   System calls (functions provided by the kernel)\n");
        printf("3   Library calls (functions within program libraries)\n");
        printf("4   Special files (usually found in /dev)\n");
        printf("5   File formats and conventions eg /etc/passwd\n");
        printf("6   Games\n");
        printf("7   Miscellaneous (including macro packages and conventions), e.g.\n");
        printf("    man(7), groff(7)\n");
        printf("8   System administration commands (usually only for root)\n");
        printf("9   Kernel routines [Non standard]\n");
        goto END;
    }else{
        goto BAD;
    }
    END:
    return 0;
    BAD:
    return -1;
}


