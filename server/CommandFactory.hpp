/*
-------------------------------------
CLASA COMMANDFACTORY
- comenzile sunt siruri de caractere delimitate de new line;
- comunicarea se face prin executia de comenzi citite de la tastatura in client si executate in procesele copil create de server;
Serverul citeste o linie text, trebuie sa identifice comanda din acea linie si sa apeleze logica potrivita
-------------------------------------
*/

#pragma once

#include <string>
#include <memory>
#include "Command.hpp"

using namespace std;
/*metoda statica->poate fi apelata fara instanta
primeste sirul de comanda complet de la client si returneaza
un unique_ptr spre obiectul potrivit
Server ul creeaza o comanda, o executa, apoi o lasa sa se distruga automat
*/
class CommandFactory{
    public:
        static unique_ptr<Command> create(const string& commandString);
        
};