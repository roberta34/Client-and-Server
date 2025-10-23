/*
----------------------------------
CLASA LOGGER
Permite inregistrarea evenimentelor
Este implementata folosind modelul Singleton
----------------------------------
*/
#pragma once

#include <string>
#include <iostream>
//urmatoarele sunt relevante pt timestamp
#include <chrono>
#include <ctime> //pentru time_t, tm, localtime_r, strftime
#include <iomanip>

using namespace std;

enum class LogLevel{
    INFO,
    WARNING,
    ERROR
};

class Logger{
    private:
        Logger();//e privat pt ca e implementa
        string getTimestamp(); //genereaza ora curenta
    public:
        static Logger& getInstance(); //ofera acces global la o singura instanta de Logger
        void log(const string& message, LogLevel level=LogLevel::INFO);//luam implicit nivelul INFO
        ~Logger()=default;  
};