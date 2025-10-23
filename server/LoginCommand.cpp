#include "LoginCommand.hpp"
#include "Logger.hpp"
#include "SessionManager.hpp"
#include <fstream>
#include <sstream> //pentru a prelucra comanda login : username
#include <cctype>

using namespace std;

string LoginCommand::getName() const{
    return "login";
}

//extragem username ul de dupa login
LoginCommand::LoginCommand(const string& command){
    int pos=0, ok=0;
    for(int i=0;i<command.size() && !ok;++i){
        if (command[i]==':')
        {
            pos=i;
            ok=1;
        }
    }
    if(ok){
        username=command.substr(pos+1); //extrage partea de dupa ":"
        while(!username.empty() && isspace(username[0])) 
        {
            username.erase(0,1);//sterge primul caracter
        }
    }
}

string LoginCommand::execute(){
    Logger::getInstance().log("Login attempt for user: "+username);
    if (username.empty()) {
        Logger::getInstance().log("Empty username received for login.", LogLevel::WARNING);
        return "ERROR: Invalid syntax. Use: login : username";
    }
    ifstream fin("allowed_users.txt");
    if(!fin.is_open()){
        return "Error: File allowed_users.txt could not be opened";
    }
    string user;
    bool ok=false;
    while(getline(fin, user)){
        if (user==username){
            ok=true;
            break;
        }
    }
    if(!ok){
        Logger::getInstance().log("Failed login for the user: "+ username, LogLevel::WARNING);
        return "The user is not allowed to log in.";
    }

    SessionManager::getInstance().login(username);
    Logger::getInstance().log("User logged successfully: "+username);
    return "Successfully logged in as "+username;
}