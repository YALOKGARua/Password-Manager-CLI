#pragma once
#include <functional>
#include <vector>
#include <string>

class UI {
public:
    using Action = std::function<void()>;
    UI();
    ~UI();
    void addMenuItem(const std::string& label, Action action);
    void run();
private:
    void drawMenu();
    std::vector<std::pair<std::string, Action>> menu_;
}; 