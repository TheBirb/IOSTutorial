//
//Created by Jorge Goicoechea 30/3/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>


int main(int ac, char* av[])
{
    char *file= av[1];
    char *location= av[2];
    char newplace[300];

    if(ac!=3)
        printf("Error:\nWrong arguments\n");
    else
    {
        if(location[0]=='/')				//check to see if input is a path
        {
            strcat(location,"/");			//if argument is a Full Path, prepare to mv to end of path.
            strcat(location,file);
            if(rename(file, location)== 0){
                printf("Successful\n");
                link(file, location);
                
            }else{
                printf("Error:\nDirectory not found\n");
            }
        }
        else
        {
            DIR *isD;
            isD = opendir(location); 				// check if argument is a DIR in CWD

            if(isD==NULL)
            {
                if( rename(file,location)!= 0)
                    printf("Error: File not moved\n");
                else
                    printf("Successful\n");
            }
            else
            {
                char *ptrL;
                ptrL = getcwd(newplace, 50);		// get current working directory path 
                strcat(newplace,"/");				
                strcat(newplace,location);			// attach mv location to path ptrL
                strcat(newplace,"/");
                strcat(newplace, file);				// keep original file name
                if(rename(file,ptrL)!=-1){
                    link(file, location);
                    unlink(location);
                    
                }else
                    printf("Error:\nDirectory not found \n");
                    
                closedir(isD);
            }
        }
    }
    return 0;
}

