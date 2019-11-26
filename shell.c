#include"shell.h"
#include"shell_parse.h"
#include"shell_execute.h"
#include"shell_execute_builtin.h"
#include"shell_history.h"
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<stdio.h>
#include<wait.h>
#include<fcntl.h>


int argc=0,in,out,redirect_flag=0;
int bg=0,ppid,history_fd,pipec=0,bg_cnt=0,status;
job_list jobs;



//Normal execution ie without pipes

int normal_execute()
{
    int pid;
    if(bg==1 || redirect_flag>0 || execute_builtin(args[0][0],0)==0)
    {
        pid=fork();
        if(pid<0)
        {
            perror("Can Not execute the command");
        }
        else
        {
            if(pid==0)
            {

                signal(SIGINT,SIG_DFL);
                signal(SIGCHLD,SIG_DFL);
                signal(SIGTSTP,SIG_DFL);
                if(bg==1)
                {
                    setpgid(0,0);
                }
                if(execute_builtin(args[0][0],0))
                    exit(0);
                else
                    exit(execute(args[0][0],0));
            }
            else
            {
                jobs.job_id[jobs.cnt]=pid;
                strcpy(jobs.job_name[jobs.cnt],args[0][0]);
                jobs.cnt++;
                if(bg==0)
                {
                    
                    waitpid(pid,&status,WUNTRACED); 
                    if(WIFEXITED(status))           //detects successful exit
                        jobs.cnt--;
                    else if(WIFSIGNALED(status))    //detects ctrl c
                        jobs.cnt--;
                    else
                    {
                        printf("\n Stopped [%d] %s",jobs.job_id[jobs.cnt-1], jobs.job_name[jobs.cnt-1]);
                    }
                    
                }
                else
                {
                    printf("[%d] %d %s",jobs.cnt,jobs.job_id[jobs.cnt-1],jobs.job_name[jobs.cnt-1]);
                }
                                       
            }
                    
        }
    }
    return (EXIT_SUCCESS);    
}


//Piped execution

int piped_execution()
{
    int i,j;
    int fd[2];
	pid_t pid;
	int fdd = 0;				// Backup 

	for(i=0;i<=pipec;i++)
    {
		pipe(fd);				// Sharing bidiflow 
        //printf(" cmd = %d",i);
		if ((pid = fork()) == -1) {
			perror("fork");
			exit(1);
		}
		else if (pid == 0) 
        {
			dup2(fdd, 0);
			if (i<pipec) {
				dup2(fd[1], 1);
			}
			close(fd[0]);
			if(execute_builtin(args[i][0],i))
                    exit(0);
            else
                exit(execute(args[i][0],i));
			exit(1);
		}
		else 
        {
			wait(NULL); 		/* Collect childs */
			close(fd[1]);
			fdd = fd[0];
		}
	}
}


//to prevent infinite loop when next time arguement list is small
void setargs()
{
    int j;
	for(int i=0;i<50;i++)
		for (j = 0; j < 20; j++)
                    args[i][j]=NULL;        
    pipec=0;
    redirect_flag=0;
    free(infile);
    infile=NULL;
    free(outfile);
    outfile=NULL;
}


//update background jobs table

void update_jobs()
{
    job_list temp_jobs;
    temp_jobs.cnt=0;
    for(int i=0;i<jobs.cnt;i++)
    {
        if(kill(jobs.job_id[i],0)==0)
        {
            temp_jobs.job_id[temp_jobs.cnt]=jobs.job_id[i];
            strcpy(temp_jobs.job_name[temp_jobs.cnt],jobs.job_name[i]);
            temp_jobs.cnt++;
        }
    }
    jobs=temp_jobs;
}


//Main Shell implementation ,Command prompt etc

void main_shell_loop()
{
	int i,cmd_cnt=0,j,t,sin;
	char username[100],systemname[100],cwd[100],line[100],*cmd[100];
	gethostname(systemname,100);
	getlogin_r(username,100);
    jobs.cnt=0;
    bg_cnt=0;
    dup2(STDIN_FILENO,sin);
	while(1)
	{
        cmd_cnt=0;
        bg=0;
        update_jobs();
        ppid=getpid();
		getcwd(cwd,100);
		if(strcmp(root,cwd)==0)
			strcpy(cwd,"~");
		printf("\n%s@%s:%s >",username,systemname,cwd);
		if(fgets(line,100,stdin)==NULL)
            clearerr(stdin);
        if(line[0]=='\0' || line[0]=='\n')
			continue;
        log_history(line);
		if(strcmp(line,"exit\n")==0 || strcmp(line,"quit\n")==0)
		{
            close(history_fd);
            exit(0);
        }
        
		cmd[cmd_cnt]=strtok(line,";");
		while (cmd[cmd_cnt]!=NULL && strlen(cmd[cmd_cnt])>1)
		{
			cmd[++cmd_cnt]=strtok(NULL,";");
		}

        for(i=0;i<cmd_cnt;i++)
        {
            setargs();
            bg=0;
			parse_string(cmd[i]);
            
			if(strcmp(args[pipec][argc-1],"&")==0)
             {
                args[pipec][argc-1]=NULL;
                argc--;
                bg=1;
             }
            /*for(j=0;j<=pipec;j++)
	        {
		        for(t=0;args[j][t]!=NULL;t++)
		        {
			        printf("args[%d][%d] = %s\n",j,t,args[j][t]);
		        }
	        }
            printf("Infile %s\n",infile );
	        printf("Outfile %s\n",outfile );
	        printf("pipes %d\n",pipec );
	        printf("argc %d\n",argc );
            printf("Redirections %d\n",redirect_flag );
            printf("BackGround Jobs %d\n",jobs.cnt );
            */
           if(pipec==0)
            {
                //printf("Going for Normal");
                normal_execute();
            }
            else
            {
                piped_execution();
            }
        }
	}
}




//Signal Handler for Background and child processes and used for printing the status of the 
//background process

void proc_exit()
{
		int status,w;
        job_list jobs_temp;
        jobs_temp.cnt=0;
        w=waitpid(-1, &status, WNOHANG);
        if(w>0)
        {
            if (WIFEXITED(status))
		        printf (" process with pid %d exited normally", w);
            else
                printf (" process with pid %d exited abnormally", w);
        }
}


