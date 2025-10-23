/*
-----------------------
CLASA LOGINCOMMAND
Comand: "login : username"
Executata doar intr-un copil din server
Valideaza utilizatorul dintr-un fisier conf
Marcheaza utilizatorul ca autentificat
-----------------------
*/
#pragma once

#include "Command.hpp"

using namespace std;

class LoginCommand: public Command{
        string username;
    public:
        explicit LoginCommand(const string& command);//explicit previne conversii implicite de la string la LoginCommand
        string execute() override;
        string getName() const override;

};