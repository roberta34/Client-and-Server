/*
----------------------------
CLASA GETLOGGEDUSERSCOMMAND
-"get-logged-users" - afiseaza informatii (username, hostname for remote login, time entry was made) despre utilizatorii autentificati pe sistemul de operare
(vezi "man 5 utmp" si "man 3 getutent"). Aceasta comanda nu va putea fi executata daca utilizatorul nu este autentificat in aplicatie. 
Executia comenzii va fi realizata intr-un proces copil din server;
!Comanda nu poate fi executata daca utiilizatorul nu este autentificat in aplicatie!
----------------------------
*/

#pragma once
#include "Command.hpp"

class GetLoggedUsersCommand: public Command{
    public:
        GetLoggedUsersCommand()=default;
        string execute() override;
        string getName() const override;
};