#include "PasswordManager.hpp"
#include "UI.hpp"
#include <iostream>
#include <unordered_map>

int main(int argc, char* argv[]) {
    if(argc < 3) { std::cerr<<"Usage: pwdmgr <command> --db <dbpath> [options]\n"; return 1; }
    std::string command = argv[1], dbPath;
    for(int i=2; i<argc; ++i) if(std::string(argv[i])=="--db" && i+1<argc) dbPath = argv[++i];
    if(dbPath.empty()) { std::cerr<<"--db is required\n"; return 1; }
    std::cout<<"Master Password: "; std::string master; std::getline(std::cin, master);
    PasswordManager::Config cfg{dbPath};
    PasswordManager pm(cfg, master);
    std::unordered_map<std::string, std::function<void()>> cmds{
        {"init", [&](){ pm.initDb(); }},
        {"add",  [&](){ pm.add("", "", ""); }},
        {"list", [&](){ pm.list(); }},
        {"get",  [&](){ pm.get(""); }},
        {"remove",[&](){ pm.remove(""); }},
        {"update",[&](){ pm.update("", std::nullopt, std::nullopt); }}
    };
    try {
        if(cmds.count(command)) cmds[command]();
        else {
            UI ui; ui.addMenuItem("Init DB", [&](){ pm.initDb(); });
            ui.addMenuItem("List entries", [&](){ pm.list(); });
            ui.run();
        }
    } catch(const std::exception& e) {
        std::cerr<<"Error: "<<e.what()<<"\n";
        return 1;
    }
    return 0;
} 