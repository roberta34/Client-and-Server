/*
-------------------------
CLASA LOGOUTCOMMAND
Ne delogam
-------------------------
*/

#pragma once
#include "Command.hpp"
#include <string>

using namespace std;

class LogoutCommand : public Command{
    public:
        string execute() override;
        string getName() const override;
};