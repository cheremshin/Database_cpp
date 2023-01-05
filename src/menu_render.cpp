#include "menu.h"

#include <iostream>

void Menu::PrintMainMenu() {
    std::cout << "+----------------------------------+\n"
                 "|             Database             |\n"
                 "+----------------------------------+\n"
                 "|                                  |\n"
                 "|    1. Load database              |\n"
                 "|    2. Create new database        |\n"
                 "|    0. Exit                       |\n"
                 "|                                  |\n"
                 "+----------------------------------+\n";

    std::cout << "\n> ";
}

void Menu::PrintLoadMenu(std::vector<std::string> db_list) {
    std::cout << "+----------------------------------+\n"
                 "|             Database             |\n"
                 "+----------------------------------+\n"
                 "|          Load database           |\n"
                 "+----------------------------------+\n"
                 "|                                  |\n";

    if (db_list.size() == 0) {
        std::cout << "|        No databases found        |\n";
    } else {
        for (int i = 0; i < db_list.size(); i++) {
            std::cout << "|    " << i + 1 << ". " << db_list[i];
            for (int j = 0; j < 27 - db_list[i].length(); j++) {
                std::cout << " ";
            }
            std::cout << "|\n";
        }
    }

    std::cout << "|                                  |\n"
                 "|    0. Back                       |\n"
                 "|                                  |\n"
                 "+----------------------------------+\n";

    std::cout << "\n> ";
}

void Menu::PrintDbMenu(std::string filename) {
    std::cout << "+----------------------------------+\n"
                 "|             Database             |\n"
                 "+----------------------------------+\n"
                 "|";

    int half_len = filename.length() / 2;
    for (int i = 0; i < 17 - half_len; i++) {
        std::cout << " ";
    }
    std::cout << filename;

    half_len = (filename.length() % 2) ? half_len : half_len - 1;
    for (int i = 17 + half_len + 1; i < 34; i++) {
        std::cout << " ";
    }


    std::cout << "|\n";
    std::cout << "+----------------------------------+\n"
                 "|    1. Print                      |\n"
                 "|    2. Insert                     |\n"
                 "|    3. Delete                     |\n"
                 "|    4. Search                     |\n"
                 "|                                  |\n"
                 "|    0. Save and exit              |\n"
                 "+----------------------------------+\n";
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

void Menu::PrintChoosingFields() {
    std::cout << "\n+----------------------------------+\n"
                 "|         Chooosing fields         |\n"
                 "+----------------------------------+\n";
}

void Menu::ShowTypes() {
    std::cout << "\n  [1] - int\n"
                 "  [2] - size_t\n"
                 "  [3] - float\n"
                 "  [4] - double\n"
                 "  [5] - char\n"
                 "  [6] - string\n";

    std::cout << "\n> ";
}
