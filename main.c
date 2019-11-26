#include<stdio.h>
#include<signal.h>
#include"shell.h"
#include"shell_history.h"
#include<unistd.h>

//main function.



int main()
{
    signal(SIGINT,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);
	signal(SIGCHLD,proc_exit);
    init_history();
    getcwd(root,100);
    main_shell_loop();
}