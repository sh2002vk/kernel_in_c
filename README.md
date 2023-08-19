# Shell from scratch
[*Project*](https://github.com/sh2002vk/c_projects/blob/main/shell.c) \
A simple, unix-based shell with the following command schema:

| TYPE   | OPERATION | ARGS                 | NOTES                             | IMPLEMENTED |
|--------|-----------|----------------------|-----------------------------------|-------------|
| file   | list      | NONE                 | List all files                    | YES         |
| file   | print     | NONE                 | Print the current path            | YES         |
| file   | enter     | directory            | Go into a directory               | YES         |
| file   | makedir   | name                 | Make a new directory              | YES         |
| file   | makefile  | name                 | Make a new file in curr directory | YES         |
| system | status    | NONE                 | Get current running processes     | NO          |
| system | resources | NONE                 | Current resource use              | NO          |
| system | info      | NONE                 | Returns system information        | NO          |
| system | start     | uid, resources, time | Returns a hash to id the process  | NO          |
| system | kill      | hash                 | Ends the given process            | NO          |

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
