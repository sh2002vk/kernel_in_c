# Shell from scratch
[*Project*](https://github.com/sh2002vk/kernel_in_c/blob/main/shell.c) \
A simple, unix-based shell with the following command schema: \
[STILL FIXING BUGS, NOT COMPLETE]

TO RUN: ```gcc shell.c file_handler.c parser.c pcb.c scheduler.c -o shell.exe```

| TYPE   | OPERATION    | ARGS                      | NOTES                             | IMPLEMENTED | 
|--------|--------------|---------------------------|-----------------------------------|-------------|
| file   | list         | NONE                      | List all files                    | YES         | 
| file   | print        | NONE                      | Print the current path            | YES         |
| file   | enter        | directory                 | Go into a directory               | YES         |
| file   | makedir      | name                      | Make a new directory              | YES         |
| file   | makefile     | name                      | Make a new file in curr directory | YES         |
| system | status       | NONE                      | Get current running processes     | YES         |
| system | queue_status | uid                       | Get current queue processes       | YES         |
| system | start        | uid, priority, core, time | Returns a hash to id the process  | YES         |
| system | kill         | uid                       | Ends the given process            | YES         |

The "system" module that the shell uses is a mock process manager built from scratch. It does not actually make any
new system processes.

## Scope of the Process Manager
- Get current processes
- Get specs, like how many processes can run, etc
- Start a new process
- Kill a given process

#### Architecture of the Process Manager

##### PCB (Process Control Block)
Represents a single process, contains information on it like id, priority, cpu requirements, etc
Also updates state

##### Scheduler
Puts jobs in the execution queue

##### Context Switcher
Used to move a process into idle mode or running mode

##### Hierarchy Manager
Handles parent and children processes
