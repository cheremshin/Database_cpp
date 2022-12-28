#ifndef SRC_FILE_HANDLER_H_
#define SRC_FILE_HANDLER_H_

#include <string>
#include <vector>
#include <fstream>

#include "common.h"

class FileHandler
{
private:
    const std::string dir = "databases";
    std::vector<std::string> db_list;

    void FindDbs();
public:
    FileHandler();

    std::vector<std::string> GetList();

    std::fstream OpenRead(std::string filename);
    std::fstream OpenWrite(std::string filename);

    void WriteStructure(std::fstream &file, Structure structure);
    Structure ReadStructure(std::fstream &file);
};

#endif  // SRC_FILE_HANDLER_H_