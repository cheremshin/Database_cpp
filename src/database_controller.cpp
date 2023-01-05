#include <iostream>
#include <fstream>
#include <filesystem>
#include "database_controller.h"
#include "file_handler.h"
#include "Encoder.h"

DatabaseController::DatabaseController(std::string filename, Structure structure) {
    auto file_handler = FileHandler{};

    std::string index_file = "databases/" + filename + ".idb";
    std::string db_file = "databases/" + filename + ".db";

    if (!std::filesystem::exists(index_file)) {
        auto tmp = file_handler.OpenWrite(index_file);
        tmp.close();
    }

    if (!std::filesystem::exists(db_file)) {
        auto tmp = file_handler.OpenWrite(db_file);
        tmp.close();
    }

    this->idb = std::fstream{index_file,
                             std::ios::binary |
                             std::ios::in |
                             std::ios::out};

    this->db = std::fstream{db_file,
                            std::ios::binary |
                            std::ios::in |
                            std::ios::out};

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

TmpMemory DatabaseController::ReadFieldValue(std::vector<char> input, size_t *offset, int type) {
    auto mem = TmpMemory{};

    switch (type) {
    case VINT: {
            char bytes[sizeof(int)];
            for (int i = 0; i < sizeof(int); i++) {
                bytes[i] = input[*offset + i];
            }
            int *tmp = (int *)bytes;
            mem.v_int = *tmp;
            *offset += sizeof(int);
            break;
        }
    case VSIZE: {
            char bytes[sizeof(size_t)];
            for (int i = 0; i < sizeof(size_t); i++) {
                bytes[i] = input[*offset + i];
            }
            size_t *tmp = (size_t *)bytes;
            mem.v_size_t = *tmp;
            *offset += sizeof(size_t);
            break;
        }
    case VFLOAT: {
            char bytes[sizeof(float)];
            for (int i = 0; i < sizeof(float); i++) {
                bytes[i] = input[*offset + i];
            }
            float *tmp = (float *)bytes;
            mem.v_float = *tmp;
            *offset += sizeof(float);
            break;
        }
    case VDOUBLE: {
            char bytes[sizeof(double)];
            for (int i = 0; i < sizeof(double); i++) {
                bytes[i] = input[*offset + i];
            }
            double *tmp = (double *)bytes;
            mem.v_double = *tmp;
            *offset += sizeof(double);
            break;
        }
    case VCHAR: {
            mem.v_char = input[*offset];
            *offset += sizeof(char);
            break;
        }
    case VSTRING: {
            char bytes[sizeof(size_t)];
            for (int i = 0; i < sizeof(size_t); i++) {
                bytes[i] = input[*offset + i];
            }
            size_t *tmp = (size_t *)bytes;
            mem.v_size_t = *tmp;
            *offset += sizeof(size_t);

            mem.v_string = "";
            for (int i = 0; i < mem.v_size_t; i++) {
                mem.v_string += input[*offset + i];
            }
            *offset += mem.v_size_t * sizeof(char);
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

size_t DatabaseController::GetSize(int type) {
    size_t size = 0;

    switch (type)
    {
    case VCHAR:
        size = 1UL;
        break;
    case VINT:
    case VFLOAT:
        size = 4UL;
        break;
    case VSIZE:
    case VDOUBLE:
        size = 8UL;
        break;
    case VSTRING:
        size = 32UL;
    }

    return size;
}

int DatabaseController::Print(int count) {
    int status = 1, row = 0;
    idb.seekg(0, std::ios::beg);
    db.seekg(0, std::ios::beg);

    if (FileHandler::IsEmpty(idb)) status = 0;

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

void DatabaseController::Insert(std::vector<char> input) {
    size_t offset = 0;  // Input offset
    idb.seekg(0, std::ios::end);
    db.seekg(0, std::ios::end);

    size_t seek = db.tellg();

    TmpMemory mem = TmpMemory{};
    mem.v_char = EXIST;
    WriteFieldValue(idb, mem, VCHAR);
    offset++;

    mem = ReadFieldValue(input, &offset, VSTRING);
    WriteFieldValue(idb, mem, VSTRING);
    WriteFieldValue(db, mem, VSTRING);
    mem.v_size_t = seek;
    WriteFieldValue(idb, mem, VSIZE);

    for (int i = 0; i < structure.field_types.size() - 1; i++) {
        mem = ReadFieldValue(input, &offset, structure.field_types[i]);
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

int DatabaseController::Search(std::string id, size_t *i_seek, size_t *db_seek) {
    int status = 0, inside = 0, found = 0;
    size_t seek = 0, size = 0;

    idb.seekg(0, std::ios::beg);
    auto mem = ReadFieldValue(idb, VCHAR);
    mem = ReadFieldValue(idb, VSTRING);

    size = mem.v_size_t + sizeof(size_t);

    idb.seekg(0, std::ios::beg);
    while (!idb.eof() && !inside) {
        seek += PAGE_SIZE * size;
        idb.seekg(static_cast<std::streampos>(seek));

        mem = ReadFieldValue(idb, VCHAR);
        mem = ReadFieldValue(idb, VSTRING);

        if (Encoder::Decode(mem.v_string) >= Encoder::Decode(id)) {
            inside = 1;
        }
    }

    seek -= PAGE_SIZE * size;
    idb.seekg(static_cast<std::streampos>(seek));

    std::vector<char> input;
    GetPage(size, &input);

    for (int i = 0; i < PAGE_SIZE && !found; i++) {
        size_t offset = 0;
        auto mem = ReadFieldValue(input, &offset, VCHAR);
        mem = ReadFieldValue(input, &offset, VSTRING);
        mem = ReadFieldValue(input, &offset, VSIZE);

        if ((mem.v_char == EXIST) &&
            (mem.v_string == id)) {
            found = 1;
            *db_seek = mem.v_size_t;
            *i_seek = seek + i * size;
        }
    }

    if (found) status = 1;

    return status;
}

void DatabaseController::GetPage(size_t size, std::vector<char> *input) {
    int status = 1;
    size_t rows = 0;

    char *row = new char [size];

    while (!idb.eof() && status) {
        idb.read((char *)&row, sizeof(char) * size);
        for (size_t i = 0; i < size; i++) {
            (*input).push_back(row[i]);
        }

        rows++;
        if (rows == PAGE_SIZE) status = 0;
    }
}
