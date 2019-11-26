SUBMISSION BY:
NAME:ARCHIT GOYAL (CSD UG2 LE)
ROLL NO: 2019121005


***********************************************************************************************************************************

COMPILING AND EXECUTION
make
	./arc_shell


***********************************************************************************************************************************


# FEATURES

Supports Commands   Description																			Arguements


1.echo 				-- echo the arguements to the stdout												any number
2.cd 				--change directory																	directory	
3.pwd 				--present working directory															none
4.pinfo    			--process info of the corressponding pid											none / -pid
5.ls     			--list of files in the directory													none / -a / -l / -al / -la
6.setenv 			--sets the environment variable														var / var + value
7.getenv 			--gets the value of the environment variable										var
8.unsetenv 			--unsets the environment variable													var
9.fg  				--Starts running a stopped background process in foreground							job_no
10.bg       		--Starts running a stopped background process in background    						jon_no
11.overkill 		--Kill all background processes														none
12.jobs     		--Display all jobs in Background 													none
13.kjobs			--Sends signal to a job using job number and signal number							job_no + sig_no
14.history         	--Display history 																	none / no of entries
15 others			--all other commands through Execvp 												



Supports Background execution of jobs with '&' as the last arguement.
Supports Pipes with processes.
Support I/O Redirection
Exit and Quit command for quitting the shell
Properly handling Ctrl D, Ctrl Z, Ctrl C .
Semicolon separated inputs are accepted
Modular code


***********************************************************************************************************************
Tested On : GCC 8.3.0