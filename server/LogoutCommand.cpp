#include "LogoutCommand.hpp"
#include "SessionManager.hpp"
#include "Logger.hpp"

using namespace std;

string LogoutCommand::getName() const{
    return "logout";
}

string LogoutCommand::execute(){
    if(!SessionManager::getInstance().isAuthenticated()){
        Logger::getInstance().log("Failed logout: no user logged", LogLevel::WARNING);
        return "You are not logged.\n";
    }

    string user=SessionManager::getInstance().getUsername();
    SessionManager::getInstance().logout();
    Logger::getInstance().log("Succesfully logged out user: "+user);
    return "The user has been logged out succesfully "+user+".\n";
}


