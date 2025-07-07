#include "UI.hpp"
#include <ncurses.h>

void UI::run() {
    initscr();
    noecho();
    cbreak();
    draw();
    handleInput();
    endwin();
}

void UI::draw() {
    mvprintw(0, 0, "Password Manager");
}

void UI::handleInput() {
    int ch;
    while((ch = getch()) != 'q') {}
} 