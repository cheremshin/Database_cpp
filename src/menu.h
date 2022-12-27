#ifndef SRC_MENU_H_
#define SRC_MENU_H_

#include <stdio.h>
#include <vector>
#include <string>

// #define tc_move_cursor(X, Y) printf("\033[%d;%dH", Y, X);

class Menu
{
public:
    /* Starts menu program */
    static void StartMenuModule();

    struct Structure {
        std::vector<std::string> field_names;
        std::vector<int> field_types;
    }; 
private:
    // const int window_width = 36;
    // const int margin_top = 5;
    // Input functions

    static std::string GetStr();

    // Menu pages

    static int MainMenu(int *exit_status);  // 'Main' menu logic
    static void PrintMainMenu();  // Print main menu page

    static int LoadMenu();  // 'Load' menu logic
    static void PrintLoadMenu(std::vector<std::string> db_list);  // Print load menu page

    static int CreateMenu();  // 'Create' menu logic
    static void PrintCreateMenu();  // Print create menu page
    static void PrintChoosingFields();

    static Structure GetStructure();
    static void PrintCurrentStructure(std::vector<std::string> field_names,
                                      std::vector<int> field_types);
    static int GetType(int *status);
    static void ShowTypes();

    static void OpenDb(std::string filename, std::fstream &file, Structure structure);
    static void PrintDbMenu(std::string filename);

    // Utility functions

    static int StrToInt(std::string str, int *status);
    static void ClearScreen();  // Clear console
};

#endif  // SRC_MENU_H_