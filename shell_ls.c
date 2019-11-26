#include"shell.h"
#include<stdio.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<string.h>

//List (ls ) implementation


void ls(int cmd_num)
{
	struct dirent *de;
	DIR *dr;
	struct stat stats;
	int j,l=0,a=0;
	char * dir=".";
	char buf[300];
	struct passwd *own;
	struct group *gp;
	for(j=1;args[cmd_num][j]!=NULL;j++)
	{
		if(strcmp(args[cmd_num][j],"-l")==0)
			l=1;
		else if(strcmp(args[cmd_num][j],"-a")==0)
			a=1;
		else if(strcmp(args[cmd_num][j],"-al")==0 || strcmp(args[cmd_num][j],"-la")==0)
		{
			a=1;
			l=1;
		}
		else
		{
			dir=args[cmd_num][j];
		}
	}
	if(strcmp(dir,"~")==0)
		dr=opendir(root);
	else
		dr=opendir(dir);
	if(dr==NULL)
	{
		perror("arc_sh ls :");
		return;
	}


    while((de = readdir(dr)) != NULL) 
    {   
		if(de->d_name[0]=='.' && a==0)
			continue;
        sprintf(buf, "%s/%s", dir, de->d_name);
		if(l==0)
		{
			printf("%s\t",de->d_name);
			continue;
		}
        stat(buf, &stats);
        switch (stats.st_mode & S_IFMT) {
            case S_IFBLK:  printf("b"); break;
            case S_IFCHR:  printf("c"); break; 
            case S_IFDIR:  printf("d"); break; 
            case S_IFIFO:  printf("p"); break;
            case S_IFLNK:  printf("l"); break;
            case S_IFSOCK: printf("s"); break;
            default:       printf("-"); break;
                }
        printf( (stats.st_mode & S_IRUSR) ? "r" : "-");
        printf( (stats.st_mode & S_IWUSR) ? "w" : "-");
        printf( (stats.st_mode & S_IXUSR) ? "x" : "-");
        printf( (stats.st_mode & S_IRGRP) ? "r" : "-");
        printf( (stats.st_mode & S_IWGRP) ? "w" : "-");
        printf( (stats.st_mode & S_IXGRP) ? "x" : "-");
        printf( (stats.st_mode & S_IROTH) ? "r" : "-");
        printf( (stats.st_mode & S_IWOTH) ? "w" : "-");
        printf( (stats.st_mode & S_IXOTH) ? "x" : "-");

        printf("\t%ld ", stats.st_nlink);

        own = getpwuid(stats.st_uid);
        printf("\t%s ", own->pw_name);

        gp = getgrgid(stats.st_gid);
        printf("\t%s ", gp->gr_name);

        printf("%zu\t",stats.st_size);
        printf(" %s\t", de->d_name);
        printf(" %s\t", ctime(&stats.st_mtime));
    }
    closedir(dr);
	
}