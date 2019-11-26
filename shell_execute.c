#include"shell.h"
#include"shell_pinfo.h"
#include"shell_ls.h"
#include"shell_parse.h"
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>



//Function for executing external programs on system using Execvp()


void sys_execute(int cmd_num)
{
    if(execvp(args[cmd_num][0],args[cmd_num])<0)
       {
		 perror("arc_sh :");
		return;
	   }
}

//Function For Executing Other Command including exec()


int execute(char * cmd,int cmd_num)
{
	int i;
	if(cmd!=NULL)
	{
		redirect(cmd_num);
		//printf("%ld is the length of %s\n",strlen(cmd),cmd );
		if(strcmp(cmd,"ls")==0)
			ls(cmd_num);
		else if(strcmp(cmd,"pinfo")==0)
			pinfo(cmd_num);
		else
			sys_execute(cmd_num);
	}
    return(1);
}

