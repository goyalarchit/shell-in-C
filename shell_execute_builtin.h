//shell_execute_builtin.h
#ifndef shell_execute_builtin_h
#define shell_execute_builtin_h


//Inbuilt Echo Function
void echo(int);

//Inbuilt PWD Function
void pwd();


//Inbuilt CD
void cd(int);

//Inbuilt setevn
void set_env_var(int);

//Inbuilt unsetenv
void unset_env_var(int);

//Inbuilt getenv
void get_env_var(int);

//utility function for finding process status
char * proc_status(int );

//Inbuilt jobs
void job();


//Inbuilt kjobs
void kjob(int );


//Inbuilt fg
void fg(int);

//Inbuilt bg
void builtin_bg(int );


//Inbuilt overkill
void overkill();

//Function For Executing Builtin Commands
int execute_builtin(char *,int);




#endif