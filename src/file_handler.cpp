#include "file_handler.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

FileHandler::FileHandler() {
    bool is_dir_exists = fs::exists(dir);

    if (!is_dir_exists) {
        fs::create_directory(dir);
    }

    FindDbs();
}

std::vector<std::string> FileHandler::GetList() {
    return this->db_list;
}

void FileHandler::FindDbs() {
    for (auto const& dir_entry : fs::directory_iterator{dir}) {
        std::string tmp = dir_entry.path();

        if (tmp.find(".cdb") != std::string::npos) {
            db_list.push_back(tmp.substr(dir.length() + 1, tmp.length() - dir.length() - 5));
        }
    }
}