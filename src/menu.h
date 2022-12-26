#ifndef SRC_MENU_H_
#define SRC_MENU_H_

class Menu
{
public:
    /* Starts menu program */
    static void StartMenuModule();  
private:
    // Input functions

    static int GetInt();

    // Menu pages

    static int MainMenu(int *exit_status);  // 'Main' menu logic
    static void PrintMainMenu();  // Print main menu page
    static void LoadMenu();  // 'Load' menu logic
    static void PrintLoadMenu();  // Print load menu page

    // Utility functions

    static void ClearScreen();  // Clear console
};

#endif  // SRC_MENU_H_