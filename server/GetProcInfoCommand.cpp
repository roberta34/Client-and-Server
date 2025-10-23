#include "GetProcInfoCommand.hpp"
#include "SessionManager.hpp"
#include "Logger.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <regex>

using namespace std;

string GetProcInfoCommand::getName() const{
    return "get-proc-info";
}

GetProcInfoCommand::GetProcInfoCommand(const string& command){
    int pos=0, ok=0;
    for(int i=0;i<(int)command.size() && !ok;++i){
        if (command[i]==':')
        {
            pos=i;
            ok=1;
        }
    }
    if(ok){
        pid=command.substr(pos+1);

        while(!pid.empty() && (pid[0]==' ' || pid[0]=='\t')){
            pid.erase(0,1);
        }
        while (!pid.empty() &&
            (pid.back() == ' ' || pid.back() == '\t' || pid.back() == '\n' || pid.back() == '\r' )) {
                pid.pop_back();
        }
    }
}

string GetProcInfoCommand::execute(){
    if(!SessionManager::getInstance().isAuthenticated()){
        Logger::getInstance().log("Command get-proc-info refused: the user is not logged in", LogLevel::WARNING);
        return "Error: You must be authentified when you are executing this command.";
    }
    string filename="/proc/"+pid+"/status";
    ifstream file(filename);

    if(!file.is_open()){
        Logger::getInstance().log("Could not open the file: "+ filename, LogLevel::ERROR);
        return "Error: Could not open the file for the given PID";
    }

    string result;
    string line;

    while(getline(file, line)){
        /*string p;
        string value;
        bool found=false;
        int i=0;

        while(i<line.size() && line[i]!=' '){
            p+=line[i];
            i++;
        }

        if (p=="Name:" || p=="State:" || p=="PPid:" || p=="Uid:" || p=="VmSize:") {
            while(i<line.size() && line[i]==' '){
                i++; //sarim spatiile albe   
            }
            while(i<line.size()){
                value+=line[i];
                i++;
            }
            result+=p+" "+value+"\n";
        }*/
        istringstream iss(line);//creez un stream din linie
        string key, value;
        iss>>key;//citeste primul token pana la spatiu in key
        if (key=="Name:" || key=="State:" || key=="PPid:" || key=="Uid:" || key=="VmSize:")
        {
            getline(iss, value);
            int k=0;
            while(k<(int)value.size() && (value[k]==' ' || value[k]=='\t')){
                k++;
            }
            if(k<(int)value.size())
                value=value.substr(k);
            else
                value="";
            result+=key+" "+value+"\n";
        }
    }
    if(result.empty())
        return "No information about the given PID.\n";
    else{
        Logger::getInstance().log("Command get-proc-info was executed for PID " + pid);
        return result;
    }
}

