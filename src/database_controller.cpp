#include <iostream>
#include <fstream>
#include <filesystem>
#include "database_controller.h"
#include "file_handler.h"

DatabaseController::DatabaseController(std::string filename, Structure structure) {
    auto file_handler = FileHandler{};

    std::string index_file = "databases/" + filename + ".idb";
    std::string db_file = "databases/" + filename + ".db";

    if (!std::filesystem::exists(index_file)) {
        auto tmp = file_handler.OpenWrite(index_file);
        tmp.close();
    
        this->idb = std::fstream{index_file,
                              std::ios::binary |
                              std::ios::in |
                              std::ios::out};
    }

    if (!std::filesystem::exists(db_file)) {
        auto tmp = file_handler.OpenWrite(db_file);
        tmp.close();

        this->db = std::fstream{db_file,
                                std::ios::binary |
                                std::ios::in |
                                std::ios::out};
    }

    this->structure = structure;
}

void DatabaseController::WriteFieldValue(std::fstream& file, TmpMemory mem, int type) {
    switch (type) {
    case VINT:
        file.write((char *)&mem.v_int, sizeof(int));
        break;
    case VSIZE:
        file.write((char *)&mem.v_size_t, sizeof(size_t));
        break;
    case VFLOAT:
        file.write((char *)&mem.v_float, sizeof(float));
        break;
    case VDOUBLE:
        file.write((char *)&mem.v_double, sizeof(double));
        break;
    case VCHAR:
        file.write((char *)&mem.v_char, sizeof(char));
        break;
    case VSTRING:
        size_t str_len = mem.v_string.length();
        file.write((char *)&str_len, sizeof(size_t));
        for (int i = 0; i < str_len; i++) {
            char ch = mem.v_string[i];
            file.write((char *)&ch, sizeof(char));
        }
    }
}

TmpMemory DatabaseController::ReadFieldValue(std::fstream& file, int type) {
    auto mem = TmpMemory{};

    switch (type) {
    case VINT:
        file.read((char *)&mem.v_int, sizeof(int));
        break;
    case VSIZE:
        file.read((char *)&mem.v_size_t, sizeof(size_t));
        break;
    case VFLOAT:
        file.read((char *)&mem.v_float, sizeof(float));
        break;
    case VDOUBLE:
        file.read((char *)&mem.v_double, sizeof(double));
        break;
    case VCHAR:
        file.read((char *)&mem.v_char, sizeof(char));
        break;
    case VSTRING:
        size_t str_len;
        file.read((char *)&str_len, sizeof(size_t));
        mem.v_string = "";
        for (int i = 0; i < str_len; i++) {
            char ch;
            file.read((char *)&ch, sizeof(char));
            mem.v_string += ch;
        }
    }

    return mem;
}

void DatabaseController::PrintFieldValue(TmpMemory mem, int type) {
    switch (type) {
    case VINT:
        std::cout << mem.v_int;
        break;
    case VSIZE:
        std::cout << mem.v_size_t;
        break;
    case VFLOAT:
        std::cout << mem.v_float;
        break;
    case VDOUBLE:
        std::cout << mem.v_double;
        break;
    case VCHAR:
        std::cout << mem.v_char;
        break;
    case VSTRING:
        std::cout << mem.v_string;
        break;
    }
}

int DatabaseController::Print(int count) {
    int status = 1, row = 0;
    idb.seekg(0, std::ios::beg);
    db.seekg(0, std::ios::beg);

    while (!idb.eof() && status) {
        auto mem = ReadFieldValue(idb, VCHAR);  // Get exist indicator
        char ex = mem.v_char;

        mem = ReadFieldValue(idb, VSTRING);  // Get id string
        mem = ReadFieldValue(idb, VSIZE);  // Get seek in .db file

        db.seekg(mem.v_size_t, std::ios::beg);
        ReadFieldValue(db, VCHAR);  // Skip exist indicator in db
        for (int j = 0; j < structure.field_types.size(); j++) {
            mem = ReadFieldValue(db, VINT);  // Get field type
            int type = mem.v_int;

            mem = ReadFieldValue(db, type);  // Get field value

            if (ex == EXIST) {
                std::cout << "| ";
                PrintFieldValue(mem, type);
                if (j - 1 == structure.field_types.size()) {
                    std::cout << "|\n";
                }
            }
        }

        if (count != 0) {
            row++;
            if (row == count) {
                status = 0;
            }
        }
    }

    return row;
}

void DatabaseController::Insert(std::fstream& input) {
    input.seekg(0, std::ios::beg);
    idb.seekg(0, std::ios::end);
    db.seekg(0, std::ios::end);

    size_t seek = db.tellg();

    TmpMemory mem = TmpMemory{};
    mem.v_char = EXIST;
    WriteFieldValue(idb, mem, VCHAR);
    WriteFieldValue(db, mem, VCHAR);

    mem = ReadFieldValue(input, VSTRING);
    WriteFieldValue(idb, mem, VSTRING);
    WriteFieldValue(db, mem, VSTRING);
    mem.v_size_t = seek;
    WriteFieldValue(idb, mem, VSIZE);

    for (int i = 0; i < structure.field_types.size() - 1; i++) {
        mem = ReadFieldValue(input, structure.field_types[i]);
        WriteFieldValue(db, mem, structure.field_types[i]);
    }
}

int DatabaseController::Delete(std::string id) {
    int status = 0, found = 0;
    char ex = DEL;

    idb.seekg(0, std::ios::beg);
    db.seekg(0, std::ios::beg);

    while (!idb.eof() && !found) {
        ReadFieldValue(idb, VCHAR);  // Skip exitst indicator

        auto mem = ReadFieldValue(idb, VSTRING);  // Get id string
        mem = ReadFieldValue(idb, VSIZE);  // Get seek in .db file

        if (mem.v_string == id) {
            found = 1;

            // 1. Set DEL indicator in index file
            // Moves back to position where "exists" indicator starts
            idb.seekg(-(sizeof(char) +  // size of exist byte
                      sizeof(size_t) +  // size of id_len
                      mem.v_string.length() * sizeof(char) +  // size of id
                      sizeof(size_t)),  // size of seek
                      std::ios::cur);
            // Rewrite indicator to "DEL"
            idb.write((char *)&ex, sizeof(char));

            // 2. Set DEL indicator in database file 
            db.seekg(mem.v_size_t, std::ios::beg);
            db.write((char *)&ex, sizeof(char));
        }
    }

    if (found) status = 1;

    return status;
}

int DatabaseController::Search(std::string id, size_t *seek) {
    int status = 0, found = 0;

    idb.seekg(0, std::ios::beg);

    while (!idb.eof() && !found) {
        auto mem = ReadFieldValue(idb, VCHAR);
        mem = ReadFieldValue(idb, VSTRING);
        mem = ReadFieldValue(idb, VSIZE);

        if ((mem.v_char == EXIST) &&
            (mem.v_string == id)) {
            found = 1;
            *seek = mem.v_size_t;
        }
    }

    if (found) status = 1;

    return status;
}
