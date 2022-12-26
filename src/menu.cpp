#include <iostream>
#include "menu.h"

void Menu::StartMenuModule() {
    int exit_status = 0;

    /*
     *   0 - MainMenu
     *   1 - LoadMenu
     *   2 - CreateMenu
     */
    int current_page = 0;

    while (!exit_status) {
        ClearScreen();
        switch (current_page)
        {
        case 0:
            current_page = MainMenu(&exit_status);
            break;
        case 1:
            break;
        case 2:
            break;
        default:
            break;
        }
    }
}

int Menu::GetInt() {
    int value;
    std::cin >> value;
    return value;
}

int Menu::MainMenu(int *exit_status) {
    int current_page = 0;

    PrintMainMenu();
    int value = GetInt();

    switch (value)
    {
    case 1:
        std::cout << "[Loading db menu]\n";
        break;
    case 2:
        std::cout << "[Creating db menu]\n";
        break;
    case 3:
        *exit_status = 1;
        break;
    default:
        break;
    }

    return current_page;
}

void Menu::PrintMainMenu() {
    std::cout << "+----------------------------------+\n"
                 "|             Database             |\n"
                 "+----------------------------------+\n"
                 "|                                  |\n"
                 "|    1. Load database              |\n"
                 "|    2. Create new database        |\n"
                 "|    3. Exit                       |\n"
                 "|                                  |\n"
                 "+----------------------------------+\n";

    std::cout << "\n> ";
}

void Menu::ClearScreen() {
    std::cout << "\e[1;1H\e[2J";  // ANSI escape code for screen clearing
}
