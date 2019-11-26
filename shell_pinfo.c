#include"shell.h"
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>


//Implementation of pinfo command

void pinfo(int cmd_num)
{
	int pid,fd,i;
	char buf[500],print[200];
	char *c;
	for(i=0;args[cmd_num][i]!=NULL;i++);
	if(i==1)
	{
		pid=ppid;
	}
	else if(i==2)
	{
		sscanf(args[cmd_num][1],"%d",&pid);
	}
	else
	{
		fprintf(stderr,"arc_sh pinfo : takes <=1 arguement");
		return;
	}
	
	

	sprintf(buf,"/proc/%d/status",pid);
	fd=open(buf,O_RDONLY);
	if(fd<0)
	{
		perror("Can Not find Process Details");
		return;
	}
	else
	{
		printf("\n Pid -- %d",pid);
		printf("\nProcess Status --");
		read(fd,buf,500);
		for(c=strstr(buf,"State:	")+strlen("State: ");*c!='\n';c++)
			printf("%c",*c);
		printf("\nMemory --");
		for(c=strstr(buf,"VmSize")+strlen("VmSize:    ");*c!='\n';c++)
			printf("%c",*c);
		sprintf(buf,"/proc/%d/exe",pid);
		readlink(buf,print,200);
		printf("\n executable path -- %s ",print);
	}
}