#include"shell.h"
#include"shell_history.h"
#include"shell_parse.h"
#include"shell_pinfo.h"
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<signal.h>
#include<wait.h>




//Inbuilt Echo Function

void echo(int cmd_num)
{
	int i=1;
    printf("\n");
	for(i=1;args[cmd_num][i]!=NULL;i++)
		printf("%s ",args[cmd_num][i]);
}

//Inbuilt PWD Function

void pwd()
{
    char pwd[100];
	getcwd(pwd,100);
		printf("%s ",pwd);
}


//Inbuilt CD

void cd(int cmd_num)
{
	
	int i;
	for(i=0;args[cmd_num][i]!=NULL;i++);
    if(i!=2)
    {
        fprintf(stderr,"arc_sh cd : pass only 1 arguement");
        return;
    }
	if(strcmp(args[cmd_num][1],"~")==0)
		i=chdir(root);
	else
		i=chdir(args[cmd_num][1]);
	if(i!=0)
		perror("arc_sh cd: ");
}


//Inbuilt setevn

void set_env_var(int cmd_num)
{
	
	int i;
	for(i=0;args[cmd_num][i]!=NULL;i++);
    if(i<2 || i > 3)
    {
        fprintf(stderr,"arc_sh setenv : pass either 1 or 2 arguement namely variable and value");
        return;
    }
	if(i==2)
		i=setenv(args[cmd_num][1]," ",1);
	else
		i=setenv(args[cmd_num][1],args[cmd_num][2],1);
	if(i!=0)
		perror("arc_sh setenv: ");
}


//Inbuilt unsetenv

void unset_env_var(int cmd_num)
{
	
	int i;
	for(i=0;args[cmd_num][i]!=NULL;i++);
    if(i!=2)
    {
        fprintf(stderr,"arc_sh unsetenv : pass only 1 arguement");
        return;
    }
	else
		i=unsetenv(args[cmd_num][1]);
	if(i!=0)
		perror("arc_sh unsetenv: ");
}



//Inbuilt getenv

void get_env_var(int cmd_num)
{
	
	int i;
	char * s;
	for(i=0;args[cmd_num][i]!=NULL;i++);
    if(i!=2)
    {
        fprintf(stderr,"arc_sh getenv : pass only 1 arguement");
        return;
    }
	else
	{
		s=getenv(args[cmd_num][1]);
	}
	if(s==NULL)
		printf("arc_sh getenv: No Such Environment Variable");
	else
		printf("%s",s);
	
}



//utility function for finding process status

char * proc_status(int pid)
{
	int fd;
	char buf[500],*c;
	char * status = (char *) malloc(100* sizeof(char));
	sprintf(buf,"/proc/%d/status",pid);
	fd=open(buf,O_RDONLY);
	if(fd<0)
	{
		return NULL;
	}
	else
	{
		int s_char=0;
		read(fd,buf,500);
		for(c=strstr(buf,"State:	")+strlen("State: ");*c!='\n';c++)
			status[s_char++]=*c;
		status[s_char++]='\0';
		return status;
	}
}


//Inbuilt jobs

void job()
{
	int i=0;
	char * status;
	for(i=0;i<jobs.cnt;i++)
	{
		status=proc_status(jobs.job_id[i]);
		if(status!=NULL)
			printf("\n [%d] %s %s with pid %d ",i+1,status,jobs.job_name[i],jobs.job_id[i]);
	}
}



//Inbuilt kjobs

void kjob(int cmd_num)
{
	int i,sig_no;
	for(i=0;args[cmd_num][i]!=NULL;i++);
    if(i != 3)
    {
        fprintf(stderr,"arc_sh kjobs : pass 2 arguement namely job number and signal");
        return;
    }
	else
	{
		sscanf(args[cmd_num][1],"%d",&i);
		sscanf(args[cmd_num][2],"%d",&sig_no);
		i=i-1;
		if(i<jobs.cnt)
		{
			i=kill(jobs.job_id[i],sig_no);
			printf("\n [%d] killed %s with pid %d ",i+1,jobs.job_name[i],jobs.job_id[i]);
		}
		else
		{
			printf("No Such Job found");
			return;
		}
	}
	if(i!=0)
		perror("arc_sh setenv: ");
}



//Inbuilt fg

void fg(int cmd_num)
{
	
	int i,status;
	char * s;
	for(i=0;args[cmd_num][i]!=NULL;i++);
    if(i!=2)
    {
        fprintf(stderr,"arc_sh fg : pass only 1 arguement");
        return;
    }
	else
	{
		sscanf(args[cmd_num][1],"%d",&i);
		i=i-1;
		if(i<jobs.cnt)
		{
			printf("\n [%d] Done %s with pid %d ",i+1,jobs.job_name[i],jobs.job_id[i]);
			kill(jobs.job_id[i],SIGCONT);			
			waitpid(jobs.job_id[i],&status,WUNTRACED);
		}
		else
		{
			printf("No Such Job found");
		}
	}
	
}



//Inbuilt bg

void builtin_bg(int cmd_num)
{
	
	int i,status;
	char * s;
	for(i=0;args[cmd_num][i]!=NULL;i++);
    if(i!=2)
    {
        fprintf(stderr,"arc_sh bg : pass only 1 arguement");
        return;
    }
	else
	{
		sscanf(args[cmd_num][1],"%d",&i);
		i=i-1;
		if(i<jobs.cnt)
		{
			printf("\n [%d] Running %s with pid %d in background",i+1,jobs.job_name[i],jobs.job_id[i]);
			kill(jobs.job_id[i],SIGCONT);			
		}
		else
		{
			printf("No Such Job found");
		}
	}
}



//Inbuilt overkill

void overkill()
{
	for(int i=0;i<jobs.cnt;i++)
	{
		kill(jobs.job_id[i],SIGKILL);
	}
}



//Function For Executing Builtin Commands
int execute_builtin(char * cmd,int cmd_num)
{
	int i;
	if(cmd!=NULL)
	{
		redirect(cmd_num);
		//printf("%ld is the length of %s\n",strlen(cmd),cmd );
		if(strcmp(cmd,"echo")==0)
		{
			echo(cmd_num);
		}
		else if(strcmp(cmd,"pwd")==0)
			pwd();
		else if(strcmp(cmd,"cd")==0)
			cd(cmd_num);
        else if(strcmp(cmd,"history")==0)
		    {
				for(i=0;args[cmd_num][i]!=NULL;i++);
                if(i==1)
                    view_history(10);
                else if(i==2)
                {
                    sscanf(args[cmd_num][1],"%d",&i);
                    view_history(i);
                }
                else
                {
                    fprintf(stderr,"arc_sh history : takes <=1 arguement");
                }
            }
		else if(strcmp(cmd,"setenv")==0)
			set_env_var(cmd_num);
        else if(strcmp(cmd,"unsetenv")==0)
			unset_env_var(cmd_num);
		else if(strcmp(cmd,"getenv")==0)
			get_env_var(cmd_num);
		else if(strcmp(cmd,"jobs")==0)
			job();
		else if(strcmp(cmd,"kjobs")==0)
			kjob(cmd_num);
		else if(strcmp(cmd,"fg")==0)
			fg(cmd_num);
		else if(strcmp(cmd,"bg")==0)
			builtin_bg(cmd_num);
		else if(strcmp(cmd,"overkill")==0)
			overkill();
		else
			return(0);
	}
    return(1);
}
