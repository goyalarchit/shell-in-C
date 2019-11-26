//shell_execute.h
#ifndef shell_execute_h
#define shell_execute_h

//Function For Executing Other Command including exec()
int execute(char * cmd,int cmd_num);

//Function for executing external programs on system using Execvp()
void sys_execute(int cmd_num);



#endif