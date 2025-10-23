/*
----------------------------
CLASA QUITCOMMAND
Semnaleaza inchiderea sesiunii clientului.
Serverul trimite clinetului mesajul de inchidere
----------------------------
*/

#pragma once
#include "Command.hpp"
#include <string>

class QuitCommand : public Command{
    public:
        QuitCommand()=default;
        string execute() override;
        string getName() const override;
}; 