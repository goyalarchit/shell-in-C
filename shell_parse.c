#include"shell.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<wait.h>
#include<fcntl.h>
#include<unistd.h>



//Function To Parse The Command

void parse_string(char * str)
{
	int redir_f;
	char *s;
	argc=0;
	s=strtok(str," ' \"\n");
	//printf("%d\n",strlen(s));
	//args[0]=(char *)malloc(strlen(s));
	//strcpy(args[0],s);
	while(s!=NULL)
	{
		if(strcmp(s,"<")==0 || strcmp(s,">>")==0 || strcmp(s,">")==0)
			redirect_flag++;
		if(strcmp(s,"|")==0)
			{
				args[pipec][argc]=NULL;	
				pipec++;
				argc=0;
				//printf("\n Found Pipe");
			}
		else
		{
			args[pipec][argc]=(char *)malloc(100);
			strcpy(args[pipec][argc++],s);		
		}
		//printf("%s %zu\n",args[argc],strlen(args[argc]));
		s=strtok(NULL," ' \"\n");
	}
	//printf("parsed\n");
	
}



int redirect(int cmd_no)
{
	//printf("In redirection");
	int append_flag=0,j=0;
	char *s;
	argc=0;
	for(int i=0;args[cmd_no][i]!=NULL;i++)
	{
		if(strcmp(args[cmd_no][i],"<")==0)
			{
				infile=(char *)malloc(1*100);
				strcpy(infile,args[cmd_no][++i]);
				//printf("\n %s",infile);
			}
		else if(strcmp(args[cmd_no][i],">")==0)
			{
				outfile=(char *)malloc(1*100);
				strcpy(outfile,args[cmd_no][++i]);
				//printf("\n %s",outfile);
			}
		else if(strcmp(args[cmd_no][i],">>")==0)
			{
				outfile=(char *)malloc(1*100);
				strcpy(outfile,args[cmd_no][++i]);
				append_flag=1;
				//printf("\n %s",outfile);
			}
		else
		{
			args[cmd_no][j++]=args[cmd_no][i];
		}
	}
	args[cmd_no][j]=NULL;
	/*for(int i=0;args[cmd_no][i]!=NULL;i++)
		        {
			        printf("args[%d][%d] = %s\n",cmd_no,i,args[cmd_no][i]);
		        }*/
	if(infile!=NULL)
    {
		fflush(stdin);
    	in=open(infile,O_RDONLY);
		if(in<0)
		{
			perror("Can't Open file");
			exit(0);
		}
    	if(dup2(in,0)<0)
        {
            perror("arc_shell:");
        }
        close(in);
		redirect_flag--;
    }
    if(outfile!=NULL)
    {
		fflush(stdout);
		if(append_flag==0)
			out=open(outfile,O_WRONLY|O_CREAT|O_TRUNC,0644);
    	else
			out=open(outfile,O_WRONLY|O_APPEND|O_CREAT,0644);
        if(out<0)
		{
			perror("Can't Open/Create file");
			exit(0);
		}
		if(dup2(out,1)<0)
        {
            perror("arc_shell:");
        }
        close(out);     
		redirect_flag--;       
    }
	return 1;
}