    /*
    -----------------
    CE FACE CLIENTUL 
    -----------------

    - comunicarea se face prin executia de comenzi citite de la tastatura in client pe care le trimite serverului
    - rezultatul obtinut in urma executiei oricarei comenzi va fi afisat de client;
    - comenzile sunt siruri delimitate de '\n'
    - raspunsurile sunt siruri de octeti prefixate de lungimea raspunsului;

    ---------------------

    - creeaza FIFO urile pt comunicare
    - asteapta input de la tastatura
    - trimite linia citita de catre server prin FIFO
    - afiseaza rezultatul in consola
    - se opreste cand intalneste comanda quit
    */

    #include <unistd.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/types.h>//pt fork/waitpid
    #include <sys/wait.h>
    #include <string.h>
    #include <sys/stat.h> //pt mkfifo
    #include <fcntl.h> //pt open
    #include <string>
    #include <string.h>
    #include <iostream>
    #include <arpa/inet.h>

    #define MAX_LINE 1024
    #define MAX_PAYLOAD 4096

    using namespace std;

    //- raspunsurile sunt siruri de octeti prefixate de lungimea raspunsului;


    /*definim numele fifo-urilor prin care comunicam*/
    static const char* FifoRequest="/tmp/cmd_request.fifo"; //clint->server(trimite comanda)
    static const char* FifoResponse="/tmp/cmd_response.fifo";//server->client(trimite raspunsul)

    int read_n(int fd, char *buf, int n){   //citeste exact n octeti din fd in bufferul buf
        int left=n;//atatia octeti mai avem de citit
        char *p=buf; //prin p putem avansa dupa fiecare citire partiala
        while(left>0){
            int r=read(fd, p, left);
            if (r<0){
                fprintf(stderr,"Error at read");
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
        if(left==0)
            return n;
        else return -1;
    }

    int main()
    {
        /*mkfifo(FifoRequest, 0666);
        mkfifo(FifoResponse, 0666);*/

        int fd_request=open(FifoRequest, O_WRONLY);//scriem comenzi catre server
        if (fd_request<0){
            fprintf(stderr,"open FifoRequest");
            return 1;   
        }
        int fd_response=open(FifoResponse, O_RDONLY);//citim raspunsurile de la server
        if (fd_response<0){
            fprintf(stderr,"open FifoResponse");
            return 1;
        }

        string line;//aici se va stoca comanda introdusa de utilizator
        while(1){
            cout<<"> "<<flush; //afiseaza un prompt
            if(!getline(cin, line)) 
                break;  //astept ca utilizatorul sa introduca o linie
                
            
            //"- comenzile sunt siruri de caractere delimitate de new line;"
            
            line+='\n';

            if(write_n(fd_request, line.c_str(), line.size())<0){
                //un pointer la continutul sirului->c.str()
                fprintf(stderr, "Error at sending.\n");
                break;
            }

            //primeste raspuns prefixat de lungimea sa
            //uint32_t=un intreg fara semn pe 32 de biti
            //net=vine de la network order(big endian)
            uint32_t n_net=0;  //aici stocam lungimea raspunsului 
            //citeste 4 octeti din fd_response si stocheaza in n_net
            //(char*)=se face cast explicit deoarece read() lucreaza cu void* sau char*
            int r=read_n(fd_response, (char*)&n_net, 4);//se asigura ca sunt cititi 4 octeti
            if(r!=4){
                perror("error of the length at the answer");
                break;
            }

            //ntohl= converteste un uint32_t de la network-order la host-order, rezultatul n este un numar care spune cati bytes urmeaza in mesaj
            uint32_t n=ntohl(n_net);
            if(n>=MAX_PAYLOAD){
                fprintf(stderr, "Too large answer (maximum %d bytes).\n", MAX_PAYLOAD);
                break;
            }

            char payload[MAX_PAYLOAD+1]={0};

            if (n>0){
                if (read_n(fd_response, payload,n)!= (int)n){
                    fprintf(stderr, "Eroare la citirea payload-ului.\n");
                    break;
                }
            }
            //afisam raspunsul 
            cout<<payload<<endl;
            if(line=="quit\n")
                break;
        }
        close(fd_request);
        close(fd_response);
        return 0;
    }
