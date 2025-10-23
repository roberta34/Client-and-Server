/*
------------------------------------
CE FACE SERVERUL
- comunicarea se face prin executia de comenzi citite de la tastatura in client si executate in procesele copil create de server;
- procesele copil din server nu comunica direct cu clientul, ci doar cu procesul parinte;
- in implementarea comenzilor nu se va utiliza nicio functie din familia "exec()" (sau alta similara, de ex. popen(), system()...) in vederea executiei unor comenzi de sistem ce ofera functionalitatile respective;
- comunicarea intre procese se va face folosind cel putin o data fiecare din urmatoarele mecanisme ce permit comunicarea: pipe-uri, fifo-uri si socketpair.
------------------------------------
*/


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <cstring>
#include <memory>
#include <sys/socket.h>
#include <algorithm>

#include "Command.hpp"
#include "CommandFactory.hpp"
#include "SessionManager.hpp"
#include "Logger.hpp"

//- raspunsurile sunt siruri de octeti prefixate de lungimea raspunsului;
#define MAX_LINE 1024
#define MAX_PAYLOAD_LEN 4096

const char* FifoRequest="/tmp/cmd_request.fifo"; //client->server(trimite comanda)
const char* FifoResponse="/tmp/cmd_response.fifo";//server->client(trimite raspunsul)


//- raspunsurile sunt siruri de octeti prefixate de lungimea raspunsului;

int read_n(int fd, char *buf, int n){   //citeste exact n octeti din fd in bufferul buf
   int left=n;//atatia octeti mai avem de citit
   char *p=buf; //prin p putem avansa dupa fiecare citire partiala
   while(left>0){
       int r=read(fd, p, left);
       if (r<0){
           perror("Error at read");
           return -1;
       }
       if (r==0) break;//nu mai sunt date de citit
       left=left-r;
       p=p+r;//avansam pointerul in buffer
   }
   return n-left; //returneaza cati octeti s-au citit
}


int write_n(int fd, const char* buf, int n){
   int left=n;
   const char*p=buf;
   while(left>0){
       int w=write(fd, p, left);
       if(w<0){
           perror("Error at write");
           return -1;
       }
       if (w==0) break;
       left=left-w;
       p=p+w;
   }
   return n; //returneaza asta daca s-au scris toti octetii, daca nu este eroare
}

int main() {
    
    Logger::getInstance().log("Server started!");

    mkfifo(FifoRequest, 0666);
    mkfifo(FifoResponse, 0666);
    int fd_request=open(FifoRequest, O_RDONLY);
    if(fd_request<0){
        fprintf(stderr, "Open FifoRequest");
        return 1;
    }

    int fd_response=open(FifoResponse, O_WRONLY);
    if (fd_response<0){
        fprintf(stderr, "Open FifoResponse");
        return 1;
    }

    char buffer[MAX_LINE];

    while(1){
        memset(buffer, 0, sizeof(buffer)); //seteaza toti octetii dintr-un bloc de memorie la o anumita valoare
        int by_read=0;

        //citim comanda de la client
        while(true){
            char ch;
            int r=read(fd_request, &ch, 1);
            if(r<=0) break;
            buffer[by_read++]=ch;
            if(ch=='\n' || by_read==MAX_LINE-1) break;
        }

        if(by_read==0) continue;
        string command_line=buffer;
        if (!command_line.empty() && command_line.back() == '\n')
            command_line.pop_back();
        Logger::getInstance().log("Received command: " + command_line);

        auto command=CommandFactory::create(command_line);
        //daca comanda este invalida, trimite un mesaj de eroare inapoi prin FIFO
        if(!command){
            string error_msg="Invalid command\n";
            uint32_t len=htonl(error_msg.size());//converteste valoarea lungimii in formatul gazda intr un format standardizat de retea
            //se scriu 4 octeti in FIFO
            //se trimite mai intai lungimea, ca clientul sa stie cati octeti sa citeasca mai departe pt mesaj
            write_n(fd_response, (char*)&len, 4);
            //se trimit cei 16 octeti ai mesajului din error_msg
            write_n(fd_response, error_msg.c_str(), error_msg.size());
            continue;
        }

        string cmd_name=command->getName();

        if(cmd_name=="login" || cmd_name=="get-proc-info" || cmd_name=="get-logged-users"){
            //facem pipe
            int c2p[2]; //copil->parinte
            pipe(c2p);

            pid_t pid=fork();
            if(pid==0){
                //suntem in copil0
                close(c2p[0]); //inchidem citirea de la copil la parinte
                dup2(c2p[1], STDOUT_FILENO);
                close(c2p[1]);

                string result=command->execute();
                cout<<result;
                cout.flush();
                exit(0);
            }
            else{
                //suntem in parinte
                close(c2p[1]);//inchidem scrierea din copil in parinte

                char result[MAX_PAYLOAD_LEN]={0};
                int n=read(c2p[0], result, MAX_PAYLOAD_LEN);
                waitpid(pid,NULL,0);
                close(c2p[0]);
                string response(result,n);

                if(cmd_name=="login"){
                    string username=command_line.substr(command_line.find(":")+1);
                    while(!username.empty() && (username[0]==' ' || username[0]=='\t')){
                        username.erase(0,1);
                    }
                    while(!username.empty() && (username[username.size()-1]==' ' || username[username.size()-1]=='\t') || username[username.size()-1]=='\n')
                        username.erase(username.size()-1,1);
                    SessionManager::getInstance().login(username);
                    Logger::getInstance().log("User "+username+" is authenticated in parent process");
                }

                uint32_t len=htonl(n);
                write_n(fd_response, (char*)&len, 4);
                write_n(fd_response, response.c_str(), response.size());
            }
        }
        else{
            int sv[2];
            socketpair(AF_UNIX, SOCK_STREAM, 0,sv);
            pid_t pid=fork();

            //copilul scrie in sv[1]
            //parintele citeste din sv[0]
            if(pid==0){
                //in copil
                close(sv[0]);
                dup2(sv[1],STDOUT_FILENO);
                close(sv[1]);

                string result=command->execute();
                cout<<result;
                cout.flush();
                exit(0);
            }
            else{
                //in parinte
                close(sv[1]);
                char result[MAX_PAYLOAD_LEN]={0};
                int n=read(sv[0], result, MAX_PAYLOAD_LEN);
                waitpid(pid,NULL,0);
                close(sv[0]);

                uint32_t len=htonl(n);
                write_n(fd_response, (char*)&len, 4);
                write_n(fd_response, result, n);
            }

            if(cmd_name=="quit"){
                Logger::getInstance().log("Received quit command. Server shutting down.");
                break;
            }
        }
    }
    close(fd_request);
    close(fd_response);

    unlink(FifoRequest);
    unlink(FifoResponse);

    Logger::getInstance().log("Server stopped succesfully");
    return 0;
}
