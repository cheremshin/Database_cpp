#include "encoder.h"
#include <algorithm>

std::string Encoder::alphabet = "0123456789"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz";
int Encoder::base = 63;

std::string Encoder::Encode(size_t input) {
    if (input < base) return std::to_string(input);

    std::string output = "";

    while (input > 0) {
        output += alphabet[input % base];
        input /= base;
    }

    std::reverse(output.begin(), output.end());

    return output;
}

size_t Encoder::Decode(std::string input) {
    size_t output = 0;

    for (int i = 0; i < input.length(); i++) {
        output = (output * base) + alphabet.find(input[i]);
    }

    return output;
}

std::string Encoder::GenerateId() {
    std::time_t time = std::time(nullptr);
    return Encode((size_t)time);
}
