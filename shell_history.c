#include"shell.h"
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

//Bonus :: Implementation of history


void init_history()
{
    history_fd=open("history.txt",O_RDWR |O_APPEND | O_CREAT,0600);
    if(history_fd<0)
    {
        perror("Can Not find Process History");
		return;
    }
    
}


void log_history(char * line)
{
    if(history_fd<0)
    {
        perror("Can Not find Process History");
		return;
    }
    else
    {
        lseek(history_fd,0,SEEK_END);
        write(history_fd,line,strlen(line));
    }
}



void view_history(int num)
{
    char c;
    int l=0;
    if(history_fd<0)
    {
        perror("Can Not find Process History");
		return;
    }
    else
    {
        lseek(history_fd,0,SEEK_END);
        while(l<num+1 && lseek(history_fd,-1,SEEK_CUR)!=-1)
        {
            read(history_fd,&c,1);
            lseek(history_fd,-1,SEEK_CUR);
            //printf("%c",c);
            if(c=='\n')
                l+=1;
        }
        read(history_fd,&c,1);
        while(read(history_fd,&c,1)!=0)
        {
            printf("%c",c);
        }
    }
}
