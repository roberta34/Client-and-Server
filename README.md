# Client and Server

## Overview
This project implements a client–server application in C++ that allows executing system-related commands through a structured command interface.
The client reads commands from the terminal and sends them to the server. The server:
  1. receives commands through FIFO pipes
  2. creates child processes to execute them
  3. collects the result
  4. sends the result back to the client

The project demonstrates the use of multiple Inter-Process Communication (IPC) mechanisms:
  - FIFO (named pipes) for client–server communication
  - pipe for communication between child and parent processes
  - socketpair for internal server process communication

## Architecture
The application is divided into two main components:
```
client/
    client.cpp

server/
    server.cpp
    Command.hpp
    CommandFactory.hpp
    CommandFactory.cpp
    LoginCommand.hpp
    LoginCommand.cpp
    LogoutCommand.hpp
    LogoutCommand.cpp
    GetProcInfoCommand.hpp
    GetProcInfoCommand.cpp
    GetLoggedUsersCommand.hpp
    GetLoggedUsersCommand.cpp
    QuitCommand.hpp
    QuitCommand.cpp
    SessionManager.hpp
    SessionManager.cpp
    Logger.hpp
    Logger.cpp
```
## System Flow
```
Client
  │
  │  FIFO
  ▼
Server (parent)
  │
  │ fork()
  ▼
Child process
  │
  │ execute command
  ▼
Pipe / Socketpair
  │
  ▼
Server (parent)
  │
  │ FIFO
  ▼
Client
```

## IPC Mechanisms Used
The project uses three IPC mechanisms as required:
  - FIFO - communication between client and server
  - pipe - communication between child and parent process\
  - socketpair - communication between server processes
 
 ## Communication Protocol 
 ### Sending commands
 Commands are sent as newline-terminated strings:
```
command\n
```

### Server responses
Responses are sent as: 
```
[4 bytes length][payload]
```
This allows the client to know exactly how many bytes to read.

## Implemented Commands

### login
Authenticates a user.
Format:
```
login : username
```

### get-logged-users
Displays users currently logged into the operating system.
Information is retrieved from:
```
/var/run/utmp
```

Displayed data:
  - username
  - host
  - login time

This command requires authentication.

### get-proc-info
Displays information about a specific process.
Format:
```
get-proc-info : pid
```
Data is read from:
```
/proc/<pid>/status
```

Displayed fields:
  - Name
  - State
  - Ppid
  - Uid
  - VmSize

### logout
Logs out the currently authenticated user.
```
logout
```

### quit
Closes the client session and stops the server.
```
quit
```

## Design Patterns Used

### Command Pattern
All commands derive from a common base class:
```
Command
│
├── LoginCommand
├── LogoutCommand
├── GetProcInfoCommand
├── GetLoggedUsersCommand
└── QuitCommand
```
The server executes commands generically:

```
command->execute();
```

### Factory Pattern
The CommandFactory decides which command object to create:
```
auto command = CommandFactory::create(command_line);
```

### Singleton Pattern
#### Logger
A single logger instance is used for the entire server.
```
Logger::getInstance().log("Server started");
```

#### SessionManager
Manages authentication state.
```
SessionManager::getInstance().login(username);
```

## Installation guide
### 1. Requirements
Before running the project, make sure the following tools are installed:
  - Before running the project, make sure the following tools are installed:
  - g++ compiler

To install the required build tools:
```
sudo apt update
sudo apt install build-essential
```

Verify installation:
```
g++ --version
```

### 2. Clone the Repository
Clone the project from GitHub:
```
git clone https://github.com/roberta34/Client-and-Server.git
cd Client-and-Server
```

### 3. Compile the Project

#### Compile the Client
```
g++ client/client.cpp -o client
```

#### Compile the Server
```
g++ server/server.cpp \
server/CommandFactory.cpp \
server/LoginCommand.cpp \
server/LogoutCommand.cpp \
server/GetLoggedUsersCommand.cpp \
server/GetProcInfoCommand.cpp \
server/QuitCommand.cpp \
server/SessionManager.cpp \
server/Logger.cpp \
-o server
```

### 4. Run the Server
```
./server
```

### 5. Run the Client
Open another terminal and run:
```
./client
```

## Example Session
```
> login : student
Successfully logged in as student

> get-proc-info : 1
Name: systemd
State: S
PPid: 0
Uid: 0
VmSize: 123456 kB

> get-logged-users
student | pts/0 | 10:32:15 12/05/2025

> logout
The user has been logged out successfully student.

> quit
The app is closing :(
Goodbye!
```

## Technologies Used 
  - C++
  - Linux IPC
  - FIFO (named pipes)
  - pipe
  - socketpair

## Key Features
  - Client–Server architecture
  - Multiple IPC mechanisms
  - Command-based execution system
  - Authentication mechanism
  - Process inspection functionality
  - Logged user monitoring
  - Centralized logging system


