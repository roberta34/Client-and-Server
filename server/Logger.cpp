#include "Logger.hpp"
#include <string>

Logger::Logger(){

}

//asigura o logare unitara, indiferent din ce proces al serverului e apelat Logger
Logger& Logger::getInstance(){
    static Logger instance;
    return instance;
}

string Logger::getTimestamp(){
    auto now=chrono::system_clock::now();//preia ora exacta, in biblioteca chrono
    time_t timeNow=chrono::system_clock::to_time_t(now); //converteste in formatul time_t
    tm localTime{};//obtine ora locala si o stocheaza intr-o structura tm
    localtime_r(&timeNow, &localTime);

    char buffer[30];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime); //formateaza ora intr-un string citibil de forma YYYY-MM-DD HH:MM:SS
    return string(buffer);
}

void Logger::log(const string& message, LogLevel level){
    string levelStr;
    switch(level){
        case LogLevel::INFO:
            levelStr="[INFO]";
            break;
        case LogLevel::WARNING:
            levelStr="[WARNING]";
            break;
        case LogLevel::ERROR:
            levelStr="[ERROR]";
            break;
    }
    string finalMessage=getTimestamp()+" "+levelStr+" "+message;
    cout<<finalMessage<<endl;
}