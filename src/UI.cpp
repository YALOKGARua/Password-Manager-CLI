#include "UI.hpp"
#include <ncurses.h>

UI::UI() { initscr(); noecho(); cbreak(); keypad(stdscr, TRUE); }
UI::~UI() { endwin(); }

void UI::addMenuItem(const std::string& label, Action action) { menu_.emplace_back(label, std::move(action)); }

void UI::run() {
    while(true) {
        clear(); drawMenu();
        int ch = getch();
        if(ch == 'q') break;
        if(ch >= '1' && ch < '1' + (int)menu_.size()) menu_[ch - '1'].second();
    }
}

void UI::drawMenu() {
    mvprintw(0, 0, "Select option (or q to quit):");
    for(size_t i=0; i<menu_.size(); ++i)
        mvprintw(i+2, 2, "%zu) %s", i+1, menu_[i].first.c_str());
    refresh();
} 