#include <fstream>
#include <filesystem>
#include "database_controller.h"

DatabaseController::DatabaseController(std::string filename, Menu::Structure structure) {
    if (!std::filesystem::exists("databases/" + filename + ".idb")) {
        this->idb = std::fstream{"databases/" + filename + ".idb",
                              std::ios::binary | std::ios::in | std::ios::out};
    }

    if (!std::filesystem::exists("databases/" + filename + ".db")) {
        this->db = std::fstream{"databases/" + filename + ".db",
                               std::ios::binary | std::ios::in | std::ios::out};
    }

    this->structure = structure;
}

void DatabaseController::Print() {
    
}