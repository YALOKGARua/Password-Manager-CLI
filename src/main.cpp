#include "PasswordManager.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if(argc<3) {
        std::cout<<"Usage: pwdmgr <command> --db <dbpath> [options]\n";
        return 1;
    }
    std::string command = argv[1];
    std::string dbPath;
    std::string name;
    std::string username;
    std::string password;
    for(int i=2;i<argc;i++) {
        std::string arg = argv[i];
        if(arg=="--db" && i+1<argc) dbPath=argv[++i];
        else if(arg=="--name" && i+1<argc) name=argv[++i];
        else if(arg=="--username" && i+1<argc) username=argv[++i];
        else if(arg=="--password" && i+1<argc) password=argv[++i];
    }
    std::cout<<"Master Password: ";
    std::string master;
    std::getline(std::cin, master);
    PasswordManager pm(dbPath, master);
    if(command=="init") pm.initDb();
    else if(command=="add") pm.add(name, username, password);
    else if(command=="list") pm.list();
    else if(command=="get") pm.get(name);
    else if(command=="remove") pm.remove(name);
    else if(command=="update") {
        const std::string* userPtr = username.empty()? nullptr:&username;
        const std::string* passPtr = password.empty()? nullptr:&password;
        pm.update(name, userPtr, passPtr);
    }
    return 0;
} 