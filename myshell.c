#include <stdio.h>
#include <string.h>
#include <stdlib.h>			// exit()
#include <unistd.h>			// fork(), getpid(), exec()
#include <sys/wait.h>		// wait()
#include <signal.h>			// signal()
#include <fcntl.h>	


// This function will parse the input string into multiple commands or a single command with arguments depending on the delimiter (&&, ##, >, or spaces).
char*** parseInput(char *buffer, int *type)
{
    int i=0,j=0;                // Iterators
    int commandNumber=0,argumentNumber=0;   // Self-Explanatory
    char* posDelim;             // To point to the position of delimeter, if present 
    char* token;                // To point to the individual arguments in the buffer
    char* tokenset[100];        // It will contain the array of words present in our buffer
    char*** commands;           // It will store the array of commands by parsing the tokenset

/* ---------------------- Allocation of Heap Memory to various pointers -------------------------- */
    token = (char*)malloc(sizeof(char)*100);

    for(int i=0;i<100;i++)
    {
        tokenset[i] = (char*)malloc(sizeof(char)*100);
    }

    commands = (char***)malloc(sizeof(char**)*10);
    for(int i=0;i<10;i++)
    {
        commands[i] = (char**)malloc(sizeof(char*)*10);
    }
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<10;j++)
        {
            commands[i][j] = (char*)malloc(sizeof(char)*100);
        }
    }
/* ----------------------------------------------------------------------------------------------- */
    
    // If buffer is empty, then exit the parser and continue shell
    if(buffer[0]=='\n')
    {
        *type = -2;
        return commands;
    }

    buffer[strlen(buffer)-1]='\0';      // Remove the last character ('\n') from buffer to decrease further complexities


    if(strcmp(buffer, "exit")==0)       // If the buffer contains "exit", return from parser and exit shell
    {
        *type = -1;                     // Set type = -1
        return commands;
    }

/*-------------------------------- Parse for delimiter = "&&" ----------------------------- */
    
    posDelim = strstr(buffer, "&&");    // Check if "&&" is present in the buffer
    if(posDelim)                        // If not, then posDelim = NULL, proceed for next delimiter
    {   
        *type = 1;                      // Set type = 1
        
        while((token=strsep(&buffer," "))!=NULL)    // Store all the words from buffer separated by "SPACE"   
        {                                           // in tokenset array   
            strcpy(tokenset[i], token);
            i++;                                    // 'i' counts the number of words in buffer
        }
        tokenset[i]=NULL;                           // End tokenset array with NULL
        
        while(j<i)                                  // 'j' varies from 0 to number of words
        {
            if(strcmp(tokenset[j],"&&"))            // If the jth word is not "&&", copy the word to the command argument
            {
                strcpy(commands[commandNumber][argumentNumber], tokenset[j]);
                argumentNumber++;                   // Increment the argument number
            }
            else                                                // If the word is "&&", 
            {
                commands[commandNumber][argumentNumber] = NULL; // End the current command's last argument with NULL
                commandNumber++;                                // Increase the command number 
                argumentNumber=0;                               // Set argument for new command = 0
            }

            j++;
        }
        commands[commandNumber++][argumentNumber]=NULL; // End the last command's argument with NULL
        commands[commandNumber][0]=NULL;                // End the commands array with NULL to keep track where the commands end
        
        return commands;                                // Return the commands to main function
    }
/* ---------------------------------------END---------------------------------------------- */

/*-------------------------------- Parse for delimiter = "##" ----------------------------- */
   
    posDelim = strstr(buffer, "##");    // Check if "##" is present in the buffer
    if(posDelim)                        // If not, then posDelim = NULL, proceed for next delimiter
    {
        *type = 2;                      // Set type = 2
        
        while((token=strsep(&buffer," "))!=NULL)    // Store all the words from buffer separated by "SPACE"  
        {                                           // in tokenset array 
            strcpy(tokenset[i], token);
            i++;                                    // 'i' counts the number of words in buffer
        }
        tokenset[i]=NULL;                           // End tokenset array with NULL
        
        while(j<i)                                  // 'j' varies from 0 to number of words
        {
            if(strcmp(tokenset[j],"##"))            // If the jth word is not "##", copy the word to the command argument
            {
                strcpy(commands[commandNumber][argumentNumber], tokenset[j]);
                argumentNumber++;                   // Increment the argument number
            }
            else                                                // If the word is "##",
            {
                commands[commandNumber][argumentNumber] = NULL; // End the current command's last argument with NULL
                commandNumber++;                                // Increase the command number 
                argumentNumber=0;                               // Set argument for new command = 0
            }

            j++;
        }
        commands[commandNumber++][argumentNumber]=NULL; // End the last command's argument with NULL
        commands[commandNumber][0]=NULL;                // End the commands array with NULL to keep track where the commands end
        
        return commands;                                // Return the commands to main function
    }
/* ---------------------------------------END---------------------------------------------- */

/*-------------------------------- Parse for delimeter = ">" ----------------------------- */
    
    posDelim = strstr(buffer, ">");     // Check if ">" is present in the buffer
    if(posDelim)                        // If not, then p = NULL, proceed for next delimiter
    {
        *type = 3;                      // Set type = 3
        
        while((token=strsep(&buffer," "))!=NULL)    // Store all the words from buffer separated by "SPACE" 
        {                                           // in tokenset array 
            strcpy(tokenset[i], token);
            i++;                                    // 'i' counts the number of words in buffer
        }
        tokenset[i]=NULL;                           // End tokenset array with NULL
        
        while(j<i)                                  // 'j' varies from 0 to number of words
        {
            if(strcmp(tokenset[j],">"))             // If the jth word is not ">", copy the word to the command argument
            {
                strcpy(commands[commandNumber][argumentNumber], tokenset[j]);
                argumentNumber++;                   // Increment the argument number
            }
            else                                                // If the word is ">",
            {
                commands[commandNumber][argumentNumber] = NULL; // End the current command's last argument with NULL
                commandNumber++;                                // Increase the command number 
                argumentNumber=0;                               // Set argument for new command = 0
            }

            j++;
        }
        commands[commandNumber++][argumentNumber]=NULL; // End the last command's argument with NULL
        commands[commandNumber][0]=NULL;                // End the commands array with NULL to keep track where the commands end
        
        return commands;                                // Return the commands to main function
    }
/* ---------------------------------------END---------------------------------------------- */

/* --------------------------------- Single Command --------------------------------------- */    
    
    *type = 0;                                          // Set type = 0
    
    while((token=strsep(&buffer, " "))!=NULL)           // Store all the arguments of the command directly into commands array   
    {                                                   // as number of commands = 0
        commands[commandNumber][argumentNumber] = token;    
        argumentNumber++;
    }
    commands[commandNumber++][argumentNumber]=NULL;     // End the last command's argument with NULL 
    commands[commandNumber][0]=NULL;                    // End the commands array with NULL to keep track where the commands end
    
    return commands;                                    // Return the commands to main function
}

// This function will fork a new process to execute a command
void executeCommand(char*** commands)
{
    if(!strcmp(commands[0][0], "cd"))       // If the command is "cd", don't fork
    {
        chdir(commands[0][1]);              // Execute chdir() to change the directory
    }
    else{                                   // Else fork a new child process to execute a command
        int rc = fork();

        if(rc<0)            // fork failed
        {
            exit(0);
        }
        else if(rc==0)      // child process initiated
        {
            signal(SIGINT, SIG_DFL);	// Don't Ignore SIGINT signal given by Ctrl+C
            signal(SIGTSTP, SIG_DFL);   // Don't Ignore SIGTSTP signal given by Ctrl+Z
            
            if(execvp(commands[0][0], commands[0])==-1)     //perform execvp(), returns -1 if command is incorrect
            {
                printf("Shell: Incorrect command\n");
            }
            exit(0);
        }
        else                // parent process must wait
        {
            int rc_wait = waitpid(-1, NULL, WUNTRACED);
        }
    }
    
}

// This function will run multiple commands in parallel
void executeParallelCommands(char*** commands)
{    
    for(int i=0;i<10;i++)                       // Run for multiple commands
    {
        if(commands[i][0]!=NULL)                // Check if command is present or NULL
        {
            if(!strcmp(commands[i][0], "cd"))   // If the command is "cd", don't fork
            {
                chdir(commands[i][1]);          // Execute chdir() to change the directory
            }
            else{                               // Else fork a new child process to execute a command
                int rc = fork();       // fork() returns child pid for the parent

                if(rc<0)            // fork failed
                {
                    exit(0);
                }
                else if(rc==0)      // child process initiated
                {
                    signal(SIGINT, SIG_DFL);	// Don't Ignore SIGINT signal given by Ctrl+C
                    signal(SIGTSTP, SIG_DFL);   // Don't Ignore SIGTSTP signal given by Ctrl+Z
                    
                    if(execvp(commands[i][0], commands[i])==-1)     //perform execvp(), returns -1 if command is incorrect
                    {
                        printf("Shell: Incorrect command\n");
                    }
                    exit(0);
                }
            }                       // The loop must continue to run all child processes parallely without waiting
        }
        else
        {
            break;                  // Break when the commands have been executed
        }
        
    }

    for(int i=0;i<10;i++)
    {
        if(commands[i][0]!=NULL)    // The main parent must wait for all child processes to finish
        {
            waitpid(-1, NULL, WUNTRACED);           // Wait for the ith child to complete execution
        }
        else
        {
            break;
        }
        
    }
}

// This function will run multiple commands in sequence
void executeSequentialCommands(char*** commands)
{	
    for(int i=0;i<10;i++)                       // Run for multiple commands
    {
        if(commands[i][0]!=NULL)                // Check if command is present or NULL
        {
            if(!strcmp(commands[i][0], "cd"))   // If the command is "cd", don't fork
            {
                chdir(commands[i][1]);          // Execute chdir() to change the directory
            }
            else{                               // Else fork a new child process to execute a command
                int rc = fork();

                if(rc<0)            // fork failed
                {
                    exit(0);
                }
                else if(rc==0)      // child process initiated
                {
                    signal(SIGINT, SIG_DFL);	// Don't Ignore SIGINT signal given by Ctrl+C
                    signal(SIGTSTP, SIG_DFL);   // Don't Ignore SIGTSTP signal given by Ctrl+Z
                    
                    if(execvp(commands[i][0], commands[i])==-1) //perform execvp(), returns -1 if command is incorrect
                    {
                        printf("Shell: Incorrect command\n");
                    }
                    exit(0);
                }
                else                // parent process must wait till the child process terminates for sequential execution
                {                   // i.e., the loop should wait for execution of next command
                    waitpid(-1, NULL, WUNTRACED);     
                }
            }
        }
        else
        {
            break;                  // Break when the commands have been executed
        }
        
    }
}

// This function will run a single command with output redirected to an output file specificed by user
void executeCommandRedirection(char*** commands)
{
    int rc = fork();
	
	if (rc < 0){			// fork failed; exit
		exit(0);
	}
	else if (rc == 0) {		// child (new) process

		signal(SIGINT, SIG_DFL);	// Don't Ignore SIGINT signal given by Ctrl+C
        signal(SIGTSTP, SIG_DFL);   // Don't Ignore SIGTSTP signal given by Ctrl+Z
        
        // ------- Redirecting STDOUT --------
		
		close(STDOUT_FILENO);
		open(commands[1][0], O_CREAT | O_WRONLY | O_APPEND);    // commands[1][0] contains the file to redirect output

		// -----------------------------------
	
        if(execvp(commands[0][0], commands[0])==-1) //perform execvp(), returns -1 if command is incorrect
        {
            printf("Shell: Incorrect command\n");
        }
        exit(0);

	} 
	else {              // parent process (rc holds child PID)
		int rc_wait = waitpid(-1, NULL, WUNTRACED);
	}
}

int main()
{
	// Initial declarations
    char directory[100];        // Store the present working directory path
	char *buffer;               // String to store the input buffer
	size_t bufferSize = 100;    // Size of buffer
	buffer = (char*)malloc(sizeof(char)*bufferSize);    // Allocation of space to buffer
    
    char*** commands;           // Store the parsed buffer (commands and their arguments)
    
	int type=0;                 //  Type        Description
                                //   -2         Empty Buffer 
                                //   -1         Exit Command
                                //    0         Single Command
                                //    1         Parallel Commands
                                //    2         Sequential Commands
                                //    3         Command Redirection    

	

	while(1)	// This loop will keep your shell running until user exits.
	{
        signal(SIGINT, SIG_IGN);	// Ignore SIGINT signal given by Ctrl+C
        signal(SIGTSTP, SIG_IGN);   // Ignore SIGTSTP signal given by Ctrl+Z
		
        // Print the prompt in format - currentWorkingDirectory$
		printf("%s$", getcwd(directory,100));


		// accept input with 'getcommands()'
		getline(&buffer, &bufferSize, stdin);

        // Parse input with 'strsep()' for different symbols (&&, ##, >) and for spaces.
        commands = parseInput(buffer, &type);
        
        if(type==-1)
        {
            printf("Exiting shell...\n");
			break;
        }
        
        if(type==1)
			executeParallelCommands(commands);		// This function is invoked when user wants to run multiple commands in parallel (commands separated by &&)
		else if(type==2)
			executeSequentialCommands(commands);	// This function is invoked when user wants to run multiple commands sequentially (commands separated by ##)
        else if(type==3)
			executeCommandRedirection(commands);	// This function is invoked when user wants redirect output of a single command to and output file specificed by user
		else if(type==0)
			executeCommand(commands);		// This function is invoked when user wants to run a single commands
				
    }
    return 0;
}
