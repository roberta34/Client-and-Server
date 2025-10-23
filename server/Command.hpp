/*
----------------------------------------------
CLASA ABSTRACTA COMMAND
Se definește o clasă de bază Command care descrie interfața comună a tuturor comenzilor, cu o 
metodă virtuală pură execute(). 
----------------------------------------------
Serverul trebuie sa proceseze comenzi diverse(login, get-proc-info, quit) si astfel prin folosirea 
interfetei Command creeaza doar o instanta de command si apeleaza execute().
*/

#pragma once
#include <string>

using namespace std;

class Command{
    public:
        virtual string getName() const=0;
        virtual string execute()=0;
        virtual ~Command()=default;
};