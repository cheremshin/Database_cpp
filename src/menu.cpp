#include <iostream>
#include <math.h>
#include <fstream>
#include "menu.h"
#include "file_handler.h"
#include "encoder.h"

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

    ClearScreen();
}

int Menu::MainMenu(int *exit_status) {
    int current_page = 0;

    PrintMainMenu();

    int status = 1;
    int value = StrToInt(GetStr(), &status);
    if (!status) value = -1;

    switch (value)
    {
    case 1:
        current_page = 1;
        break;
    case 2:
        current_page = 2;
        break;
    case 0:
        *exit_status = 1;
        break;
    default:
        break;
    }

    return current_page;
}

int Menu::LoadMenu() {
    int current_page = 1;

    FileHandler file_handler = FileHandler{};
    auto db_list = file_handler.GetList();

    PrintLoadMenu(db_list);
    
    int status = 1;
    int value = StrToInt(GetStr(), &status);
    if (!status) value = -1;

    if (value == 0) {
        current_page = 0;
    } else if (value > 0 && value <= db_list.size()) {
        std::fstream file = file_handler.OpenRead("databases/" + db_list[value - 1] + ".cdb");
        Structure structure = file_handler.ReadStructure(file);
        OpenDb(db_list[value - 1], file, structure);
        file.close();
    }

    return current_page;
}

void Menu::OpenDb(std::string filename, std::fstream &file, Structure structure) {
    int status = 1;
    size_t rows_count = 0, deleted = 0;

    DatabaseController db = DatabaseController(filename, structure);

    while (status) {
        ClearScreen();

        CommandPalette();
        DatabaseParams(filename, structure, rows_count, deleted);
        DatabaseValues(db, structure);

        int value = StrToInt(GetStr(), &status);
        if (!status) value = -1;

        switch (value)
        {
        case 0:
            status = 0;
            break;
        case 1:
            PrintingSection(db);
            break;
        case 2:
            InsertingSection(db, structure);
            break;
        case 3:
            DeletingSection(db);
            break;
        case 4:
            SearchingSection(db);
            break;
        default:
            break;
        }

        char c = getc(stdin);  // Pause before screen render
    }
}

void Menu::PrintingSection(DatabaseController & db) {
    int status = 1;

    ClearScreen();
    std::cout << "\nEnter number of rows to print\n"
                    "('0' - print all)\n";
    std::cout << "\n> ";

    int value = StrToInt(GetStr(), &status);
    if (!status) value = -1;

    if (value >= 0) {
        int read_rows = db.Print(value);

        if (read_rows == 0) {
            std::cout << "\nDatabase is empty\n";
        }
    } else {
        std::cout << "\nIncorrect value\n";
    }

    std::cout << "\nPress 'Enter' to get back\n";
    char ch = getc(stdin);
}

void Menu::InsertingSection(DatabaseController & db, Structure structure) {
    int status = 1;
    std::vector<char> input;

    ClearScreen();

    // Generate and push id to input array
    std::string id = Encoder::GenerateId();
    char *sz = new char[sizeof(size_t)];
    ValueToByteArray<size_t>(id.length(), &sz);
    for (int j = 0; j < sizeof(size_t); j++) {
        input.push_back(sz[j]);
    }
    for (int j = 0; j < id.length(); j++) {
        input.push_back(id[j]);
    }
    
    delete sz;

    // Get and push all db fields to input array
    for (int i = 1; i < structure.field_types.size(); i++) {
        std::cout << structure.field_names[i] << ":\n> ";

        int type = structure.field_types[i];
        switch (type)
        {
        case VINT: {
                int value = GetValue<int>();
                PushByteArray<int>(VINT, value, &input);
                break;
            }
        case VSIZE: {
                size_t value = GetValue<size_t>();
                PushByteArray<size_t>(VSIZE, value, &input);
                break;
            }
        case VFLOAT: {
                float value = GetValue<float>();
                PushByteArray<float>(VFLOAT, value, &input);
                break;
            }
        case VDOUBLE: {
                double value = GetValue<double>();
                PushByteArray<double>(VDOUBLE, value, &input);
                break;
            }
        case VCHAR: {
                char value = GetValue<char>();
                PushByteArray<char>(VCHAR, value, &input);
            }
        case VSTRING: {
                std::string bytes = GetValue<std::string>();

                char *sz = new char[sizeof(size_t)];
                ValueToByteArray<size_t>(bytes.length(), &sz);
                for (int j = 0; j < sizeof(size_t); j++) {
                    input.push_back(sz[j]);
                }
                for (int j = 0; j < bytes.length(); j++) {
                    input.push_back(bytes[j]);
                }

                delete sz;
            }
        }
    }

    db.Insert(input);
    
    
    std::cout << "\nPress 'Enter' to continue";
    char ch = getc(stdin);
}

void Menu::DeletingSection(DatabaseController & db) {
    int status = 1;

    ClearScreen();

    std::cout << "Enter 'id' of deleting element\n> ";
    std::string input = GetStr();

    status = db.Delete(input);

    if (status) {
        std::cout << "Successfully deleted\n";
    } else {
        std::cout << "No element with given 'id' found\n";
    }

    std::cout << "Press 'Enter' to continue\n";
    char ch = getc(stdin);
}

void Menu::SearchingSection(DatabaseController & db) {
    int status = 1;
    size_t idb_seek, db_seek;

    ClearScreen();

    std::cout << "Enter 'id' of searching element\n> ";
    std::string input = GetStr();

    status = db.Search(input, &idb_seek, &db_seek);

    if (status) {
        std::cout << std::endl;
        db.PrintRow(idb_seek, db_seek);
    } else {
        std::cout << "\nNo element with given 'id' found\n";
    }

    std::cout << "\nPress 'Enter' to continue\n";
    char ch = getc(stdin);
}

int Menu::CreateMenu() {
    int current_page = 2;
    FileHandler file_handler = FileHandler{};

    PrintCreateMenu();
    
    std::string filename = GetStr();
    std::fstream file = file_handler.OpenWrite("databases/" + filename + ".cdb");

    PrintChoosingFields();
    Structure structure = GetStructure();
    file_handler.WriteStructure(file, structure);

    file.close();

    current_page = 0;

    return current_page;
}

Structure Menu::GetStructure() {
    std::vector<std::string> field_names;
    std::vector<int> field_types;

    field_names.push_back("id");
    field_types.push_back(VSTRING);

    int status = 1;
    while (status) {
        ClearScreen();
        PrintCurrentStructure(field_names, field_types);

        std::cout << "\nEnter field name"
                     "\n> ";
        field_names.push_back(GetStr());


        int type = GetType(&status);
        if (status) {
            field_types.push_back(type);

            std::cout << "\nAdd another one?\n"
                         "    1. Yes\n"
                         "    2. No\n"
                         "\n> ";
            int value = StrToInt(GetStr(), &status);
            if (!status) value = 0;

            switch (value)
            {
            case 1:
                break;
            case 2:
            default:
                status = 0;
                break;
            }
        } else {
            field_names.pop_back();
            std::cout << "Wrong type\n";
            status = 1;
        }
    }

    Structure structure = Structure{};
    structure.field_names = field_names;
    structure.field_types = field_types;

    return structure;
}

int Menu::GetType(int *status) {
    ShowTypes();
    std::string str = GetStr();
    int value = StrToInt(str, status);

    int result = 0;
    if (*status) {
        switch (value)
        {
        case 1:
            result = VINT;
            break;
        case 2:
            result = VSIZE;
            break;
        case 3:
            result = VFLOAT;
            break;
        case 4:
            result = VDOUBLE;
            break;
        case 5:
            result = VCHAR;
            break;
        case 6:
            result = VSTRING;
            break;
        default:
            *status = 0;
            break;
        }
    }

    return result;
}

void Menu::ClearScreen() {
    std::cout << "\e[1;1H\e[2J";  // ANSI escape code for screen clearing
}

int Menu::StrToInt(std::string str, int *status) {
    int len = str.length();
    for (int i = 0; i < str.length() / 2; i++) {
        std::swap(str[i], str[len - i - 1]);
    }

    int res = 0;
    for (int i = 0; i < str.length() && *status; i++) {
        if (isdigit(str[i])) {
            res += (str[i] - '0') * pow(10, i);
        } else {
            *status = 0;
        }
    }

    return res;
}

std::string Menu::GetStr() {
    std::string value;
    std::cin >> value;
    return value;
}

template<typename T>
T Menu::GetValue() {
    T value;
    std::cin >> value;
    return value;
}

template<typename T>
void Menu::PushByteArray(int type, T value, std::vector<char> *input) {
    char *bytes = new char[sizeof(T)];
    ValueToByteArray<T>(value, &bytes);
    for (int i = 0; i < sizeof(T); i++) {
        (*input).push_back(bytes[i]);
    }
    delete bytes;
}

template<typename T>
void Menu::ValueToByteArray(T value, char *bytes[]) {
    std::copy(static_cast<char *>(static_cast<void *>(&value)),
              static_cast<char *>(static_cast<void *>(&value) + sizeof(value)),
              *bytes);
}

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

void Menu::CommandPalette() {
    std::cout << "\n"
                 " +--------------------------+\n"
                 " |     Command palette      |\n"
                 " +--------------------------+\n"
                 " |                          |\n"
                 " |     1. Insert            |\n"
                 " |     2. Search            |\n"
                 " |     3. Recovery          |\n"
                 " |                          |\n"
                 " |     0. Save and Exit     |\n"
                 " |                          |\n"
                 " +--------------------------+";
}

void Menu::DatabaseParams(std::string filename, Structure structure,
                          size_t rows, size_t deleted) {
    int x = 35, y = 2, width = 29;

    // horizontal lines + headers + fields
    int height = 5 + 4 + structure.field_names.size();
    for (int i = 0; i < height; i++) {
        if (i == 0 || i == 2 || i == 4 || i == height - 4 || i == height - 1) {
            PrintLine(x, y + i, width);
            if (i != 0 && i != height - 1) {
                tc_move_cursor(x + width / 2, y + i);
                std::cout << "+";
            }
        } else {
            tc_move_cursor(x, y + i);
            std::cout << "|";
            tc_move_cursor(x + width - 1, y + i);
            std::cout << "|";
            if (i == 3 || (i > 4 && i < 5 + structure.field_names.size())) {
                tc_move_cursor(x + width / 2, y + i);
                std::cout << "|";
            }
        }
    }
    tc_move_cursor(x + width / 2 - filename.length() / 2, y + 1);
    std::cout << filename;
    tc_move_cursor(x + 2, y + 3);
    std::cout << "Column name";
    tc_move_cursor(x + width / 2 + 2, y + 3);
    std::cout << "Column type";
    for (int i = 0; i < structure.field_names.size(); i++) {
        tc_move_cursor(x + 2, y + 5 + i);
        std::cout << structure.field_names[i];
        tc_move_cursor(x + width / 2 + 2, y + 5 + i);
        std::cout << DatabaseController::TypeToStr(structure.field_types[i]);
    }
    tc_move_cursor(x + 2, y + height - 3);
    printf("Rows count:    %10zu", rows);
    tc_move_cursor(x + 2, y + height - 2);
    printf("Deleted:       %10zu", deleted);
    tc_move_cursor(1, 1);
}

void Menu::DatabaseValues(DatabaseController & db, Structure structure) {
    int x = 2, y = 12 + structure.field_names.size();
    tc_move_cursor(x, y);
    std::cout << "+------------------------------------------------------------------------+";
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

void Menu::PrintCurrentStructure(std::vector<std::string> field_names,
                                      std::vector<int> field_types) {
    for (int i = 0; i < field_names.size(); i++) {
        std::cout << " | " + field_names[i] +
                     " [" + DatabaseController::TypeToStr(field_types[i]) + "]";
    }
    std::cout << " |" << std::endl;
}

void Menu::PrintLine(int x, int y, int width) {
    tc_move_cursor(x, y);

    for (int i = 0; i < width; i++) {
        if (i == 0 || i == width - 1) {
            std::cout << "+";
        } else {
            std::cout << "-";
        }
    }
}
