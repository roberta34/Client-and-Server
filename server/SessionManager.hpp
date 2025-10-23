/*
-------------------
CLASA SESSIONMANAGER
Gestioneaza sesiunile de utilizatori autentificati
Model Singleton
Retine daca un utilizator este logat sau nu
poate face login(username) si logout
-------------------
*/

#pragma once
#include <string>

using namespace std;

class SessionManager{
    private:
        string currentUser;
        SessionManager();
    public:
        static SessionManager& getInstance(); 
        void login(const string& username); 
        void logout();
        bool isAuthenticated() const;
        string getUsername() const;
        
};