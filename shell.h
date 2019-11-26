//shell.h
#ifndef shell_h
#define shell_h

typedef struct job_list
{
    int cnt ;
    int job_id[1000];
    char job_name[1000][50];
}job_list;

extern job_list jobs;


char root[100];
char arg[100];
char *args[50][20], * outfile,* infile;
extern int argc , in, out,pipec,redirect_flag,bg_cnt;
extern int bg,ppid,history_fd;


//Normal execution ie without pipes
int normal_execute();

//Piped execution
int piped_execution();


//to prevent infinite loop when next time arguement list is small
void setargs();


//update background jobs table
void update_jobs();


//Main Shell implementation ,Command prompt etc
void main_shell_loop();

//Signal Handler for Background and child processes and used for printing the status of the 
//background process
void proc_exit();


#endif