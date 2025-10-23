#include "SessionManager.hpp"
#include "Logger.hpp"

SessionManager::SessionManager(): currentUser("") {}

SessionManager& SessionManager::getInstance(){
    static SessionManager instance;
    return instance;
}

void SessionManager::login(const string& username){
    currentUser=username;
}

void SessionManager::logout(){
    currentUser="";
}

bool SessionManager::isAuthenticated() const{
    return !currentUser.empty();
}

string SessionManager::getUsername() const{
    return currentUser;
}