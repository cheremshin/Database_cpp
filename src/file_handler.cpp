#include <filesystem>
#include <iostream>

#include "file_handler.h"
#include "database_controller.h"

namespace fs = std::filesystem;

FileHandler::FileHandler() {
    bool is_dir_exists = fs::exists(dir);

    if (!is_dir_exists) {
        fs::create_directory(dir);
    }

    this->db_list.clear();
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

std::fstream FileHandler::OpenRead(std::string filename) {
    std::fstream in;
    in.open(filename, std::ios::in | std::ios::binary);
    return in;
}

std::fstream FileHandler::OpenWrite(std::string filename) {
    std::fstream out;
    out.open(filename, std::ios::out | std::ios::binary);
    return out;
}

void FileHandler::WriteStructure(std::fstream &file, Structure structure) {
    size_t len = structure.field_names.size();
    file.write((char *)&len, sizeof(size_t));

    for (int i = 0; i < structure.field_names.size(); i++) {
        // Write field name
        size_t name_len = structure.field_names[i].length();
        file.write((char *)&name_len, sizeof(size_t));
        for (size_t j = 0; j < name_len; j++) {
            file.write((char *)&structure.field_names[i][j], sizeof(char));
        }

        // Write field type
        file.write((char *)&structure.field_types[i], sizeof(int));
    }
}

Structure FileHandler::ReadStructure(std::fstream &file) {
    auto structure = Structure{};
    int len = 0;

    file.read((char *)&len, sizeof(size_t));
    for (int i = 0; i < len; i++) {
        std::string name = "";
        size_t name_len;

        file.read((char *)&name_len, sizeof(size_t));
        for (int j = 0; j < name_len; j++) {
            char tmp;
            file.read((char *)&tmp, sizeof(char));
            name += tmp;
        }

        structure.field_names.push_back(name);

        int type;
        file.read((char *)&type, sizeof(int));

        structure.field_types.push_back(type);
    }

    return structure;
}

bool FileHandler::IsEmpty(std::fstream &file) {
    return file.peek() == std::fstream::traits_type::eof();
}
