/*se includ toate fisierele care definesc comenzi concrete*/
#include "CommandFactory.hpp"
#include "LoginCommand.hpp"
#include "LogoutCommand.hpp"
#include "GetLoggedUsersCommand.hpp"
#include "GetProcInfoCommand.hpp"
#include "QuitCommand.hpp"

#include <sstream> //ne ajuta sa prelucram sirurile de caractere
#include <string>
using namespace std;

unique_ptr<Command> CommandFactory::create(const string& commandString){
    //eliminam \n de la string uri
    string cmd="";
    for(int i=0;i<commandString.size();++i){
        if(commandString[i]!='\n')
            cmd+=commandString[i];
    }
    //vrem sa verificam daca cmd se potriveste cu login, intrucat la login avem mai multe argumente
    string keyword="login";
    bool ok=true;
    if(cmd.size()>=keyword.size()){
        for(int i=0;i<keyword.size() && ok;++i){
            if(cmd[i]!=keyword[i]){
                ok=false;
            }
        }
    }
    else ok=false;//daca lungimea sirului de caractere dat ca parametru este mai mare decat 5, nu are sens continuam
    if(ok)
        return make_unique<LoginCommand>(commandString);
    keyword="get-proc-info";
    ok=true;
    if(cmd.size()>=keyword.size()){
        for(int i=0;i<keyword.size() && ok;++i){
            if(cmd[i]!=keyword[i]){
                ok=false;
            }
        }
    }
    else ok=false;
    if (ok)
        return make_unique<GetProcInfoCommand>(commandString); 
    if (cmd=="logout")
        return make_unique<LogoutCommand>();
    else if (cmd=="get-logged-users")
        return make_unique<GetLoggedUsersCommand>();
    else if (cmd=="quit")
        return make_unique<QuitCommand>();
    return nullptr;

}

