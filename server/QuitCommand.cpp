#include "QuitCommand.hpp"
#include "Logger.hpp"
#include "SessionManager.hpp"
#include <string>

using namespace std;

string QuitCommand::getName() const{
    return "quit";
}

string QuitCommand::execute(){
    if(SessionManager::getInstance().isAuthenticated()){
        string user=SessionManager::getInstance().getUsername();
        SessionManager::getInstance().logout();
        Logger::getInstance().log("The user " + user + " logged out before quit");
    }
    Logger::getInstance().log("Command 'quit' has been sent. The server will close the connection.");

    return "The app is closing :(\nGoodbye!\n";
}

