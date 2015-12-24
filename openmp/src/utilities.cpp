#include "utilities.h"

std::string ParseCSV(const std::string& s) {
    std::string ans;
    int token_size = 0;
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] == '0' || s[i] == '1') {
            token_size++;
            ans += s[i];
        } else if (s[i] == ',') {
            token_size = 0;
        } else {
            throw std::invalid_argument("invalid symbol in cs string");
        }
        if (token_size > 1) {
            throw std::invalid_argument("invalid cs string format");
        }
    }
    return ans;
}
