# LINUX-SHELL-PROGRAM-OS
C program which is used to create a customised linux shell/terminal using the concepts of OS such as fork, wait, etc

----------------------------------------------------------------------

To run the terminal, enter the following commands:
  gcc myshell.c
  ./a.out

----------------------------------------------------------------------

CHANGE DIRECTORY SUPPORT:

  Use normal 'cd' command with arguments for changing directory.
  Eg: "cd ./folder1Name/folder2Name"

----------------------------------------------------------------------
  
MULTIPLE COMMAND SUPPORT:

1. Separate commands using && to run parallelly.
    Eg: "./a.out && ./b.out"
2. Separate commands using ## to run sequentially.
    Eg: "./a.out ## ./b.out"

----------------------------------------------------------------------

OUTPUT REDIRECTION SUPPORT:

  Use '>' to redirect STDOUT
  Eg: "ls > info.txt"
  
----------------------------------------------------------------------
