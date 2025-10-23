/*
------------------------------
CLASA GETPROCINFOCOMMAND
Afiseaza informatii despre procesul indicat
!Comanda trebuie executata intr-un proces copil si doar daca utilizatorul este logat
------------------------------
*/

#pragma once
#include "Command.hpp"
#include <string>

using namespace std;

class GetProcInfoCommand: public Command{
    private:
        string pid; //pid ul extras din comanda primita
    public:
        explicit GetProcInfoCommand(const string& command);
        string execute() override;
        string getName() const override;

};