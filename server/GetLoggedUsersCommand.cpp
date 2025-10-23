
#include "GetLoggedUsersCommand.hpp"
#include "SessionManager.hpp"
#include "Logger.hpp"

#include <utmp.h>//pt utmp si getutent()
#include <ctime>
#include <sstream>//formeaza rezultatul ca string
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <sys/stat.h>

using namespace std;

string GetLoggedUsersCommand::getName() const{
    return "get-logged-users";
}

string GetLoggedUsersCommand::execute(){
    if (!SessionManager::getInstance().isAuthenticated()){  //verificam daca utilizatorul este logat
        Logger::getInstance().log("Command get-logged-users can not be used: the user is not logged", LogLevel::WARNING);
        return "You need to log in before using this command";
    }
    Logger::getInstance().log("The command get-logged-users is being executed");

    string result;
    struct stat st;
    
    setutent(); //resetam pointerul intern catre inceputul bazei de date utmp, unde se afla informatiile despre sesiuni logate
    Logger::getInstance().log("setutent() called succesfully");
    int found=0;
    struct utmp *entry;
    
    while((entry=getutent())!=nullptr){ //iteram peste toate inregistrarile active din utmp
        if(entry->ut_type==USER_PROCESS || entry->ut_type==LOGIN_PROCESS || entry->ut_type == INIT_PROCESS){ //ne intereseaza doar utilizatorii logati
            //extragem username ul;
            //entry->ut_user: adresa primului caracter din buffer 
            //sizeof(entry->ut_user) dimensiunea maxima a bufferului
            //strnlen(entry->ut_user, sizeof(entry->ut_user)) -> lungimea reala a textului din bufer
            string username(entry->ut_user, strnlen(entry->ut_user, sizeof(entry->ut_user))); 
            string host(entry->ut_host, strnlen(entry->ut_host, sizeof(entry->ut_host))); //extragem hostname(terminalul local)
            if(host.empty())    
                host="local"; 

            //extragem timpul si il formatam
            time_t raw_time = entry->ut_tv.tv_sec;
            struct tm *time_info = localtime(&raw_time);

            char time_buff[64];
            strftime(time_buff, sizeof(time_buff), "%H:%M:%S %d/%m/%Y", time_info);//formatam timpul cu ore,minute,secunde, zi, luna, an

            result+=username+" | "+host+" | "+time_buff+"\n";
            found=1;
        }
    }
    endutent(); //inchidem stream ul catre baza de date utmp
    
    if(result.empty())
        result="No logged users found.\n";
    return result;
}

