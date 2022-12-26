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
            current_page = LoadMenu();
            break;
        case 2:
            current_page = CreateMenu();
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
        current_page = 1;
        break;
    case 2:
        current_page = 2;
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

int Menu::LoadMenu() {
    int current_page = 1;

    PrintLoadMenu();
    int value = GetInt();

    if (value == 0) {
        current_page = 0;
    }

    return current_page;
}

void Menu::PrintLoadMenu() {
    std::cout << "+----------------------------------+\n"
                 "|             Database             |\n"
                 "+----------------------------------+\n"
                 "|          Load database           |\n"
                 "+----------------------------------+\n"
                 "|                                  |\n";


    std::cout << "DB list\n";  // Debug


    std::cout << "|                                  |\n"
                 "|    0. Back                       |\n"
                 "|                                  |\n"
                 "+----------------------------------+\n";

    std::cout << "\n> ";
}

int Menu::CreateMenu() {
    int current_page = 2;

    PrintCreateMenu();
    int value = GetInt();

    if (value == 0) {
        current_page = 0;
    }

    return current_page;
}

void Menu::PrintCreateMenu() {
    std::cout << "+----------------------------------+\n"
                 "|             Database             |\n"
                 "+----------------------------------+\n"
                 "|       Create new database        |\n"
                 "+----------------------------------+\n";

    std::cout << "\nType database name"
                 "\n> ";
}

void Menu::ClearScreen() {
    std::cout << "\e[1;1H\e[2J";  // ANSI escape code for screen clearing
}
