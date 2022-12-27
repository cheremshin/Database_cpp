#ifndef SRC_DATABASE_CONTROLLER_H_
#define SRC_DATABASE_CONTROLLER_H_

#include <string>

#ifndef size_t
#define size_t unsigned long
#endif

#define VINT       0b00000001
#define VSIZE      0b00000010
#define VFLOAT     0b00000100
#define VDOUBLE    0b00001000
#define VCHAR      0b00010000
#define VSTRING    0b00100000


struct TmpMemory {
    int v_int;
    size_t v_size_t;
    float v_float;
    double v_double;
    char v_char;
    std::string v_string;
};

class DatabaseController
{
private:
    std::string filename;
public:
    DatabaseController(std::string filename);

    void Print();
    void Insert();
    void Delete();
    void Search();
};

#endif  // SRC_DATABASE_CONTROLLER_H_