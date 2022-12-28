#ifndef SRC_DATABASE_CONTROLLER_H_
#define SRC_DATABASE_CONTROLLER_H_

#include <string>
#include <fstream>
#include "common.h"

#ifndef size_t
#define size_t unsigned long
#endif

#define VINT       0b00000001
#define VSIZE      0b00000010
#define VFLOAT     0b00000100
#define VDOUBLE    0b00001000
#define VCHAR      0b00010000
#define VSTRING    0b00100000


// Exist indicators: represents row status (is it deleted)

// Row availiable
#define EXIST      0b0
// Row deleted
#define DEL        0b1

struct TmpMemory {
    int v_int;
    size_t v_size_t;
    float v_float;
    double v_double;
    char v_char;
    std::string v_string;
    std::vector<char> input;
};

class DatabaseController
{
private:
    std::string filename;
    std::fstream idb;
    std::fstream db;
    Structure structure;

    void WriteFieldValue(std::fstream& file, TmpMemory mem, int type);
    TmpMemory ReadFieldValue(std::fstream& file, int type);
    TmpMemory ReadFieldValue(std::vector<char> input, int *offset, int type);
    void PrintFieldValue(TmpMemory mem, int type);
public:
    DatabaseController(std::string filename, Structure structure);

    // Prints the rows of the database in the amount of "count" 
    // (if 0, then the entire database)
    // return 0, if database is empty, otherwise - number of rows
    int Print(int count);

    // Adds a record from a prepared temporary file
    // Structure: [id_len][id][type](string len)[value]...
    void Insert(std::vector<char> input);

    // Marks a record with the matching "id" as deleted 
    // (will be deleted when the database is closed)
    // return values: 0 - not found, 1 - deleted
    int Delete(std::string id);

    // Search for a record with matching "id"
    // If found, set seek to "seek" pointer
    // return value: 0 - not found, 1 - found
    int Search(std::string id, size_t *seek);
};

#endif  // SRC_DATABASE_CONTROLLER_H_