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

int GetThreadRowCnt(int n, int thread_cnt) {
    return (n + thread_cnt - 1) / thread_cnt;
}

int GetFirstRowNumber(int n, int thread_cnt, int node_number) {
    int thread_row_cnt = GetThreadRowCnt(n, thread_cnt);
    return thread_row_cnt * node_number;
}

int GetLastRowNumber(int n, int thread_cnt, int node_number) {
    int thread_row_cnt = GetThreadRowCnt(n, thread_cnt);
    return std::min(GetFirstRowNumber(n, thread_cnt, node_number) + thread_row_cnt - 1, n - 1);
}
