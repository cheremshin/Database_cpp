#ifndef SRC_ENCODER_H_
#define SRC_ENCODER_H_

#include <string>
#include <ctime>

class Encoder
{
private:
    static std::string alphabet;
    static int base;

    
public:
    static std::string Encode(size_t input);
    static size_t Decode(std::string input);
    static std::string GenerateId();
};

std::string Encoder::alphabet = "0123456789"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz";
int Encoder::base = 63;

#endif  // SRC_ENCODER_H_