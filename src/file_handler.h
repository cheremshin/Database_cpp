#ifndef SRC_FILE_HANDLER_H_
#define SRC_FILE_HANDLER_H_

#include <string>
#include <vector>
#include <fstream>

class FileHandler
{
private:
    const std::string dir = "databases";
    std::vector<std::string> db_list;

    void FindDbs();
public:
    FileHandler();

    std::vector<std::string> GetList();
};

#endif  // SRC_FILE_HANDLER_H_