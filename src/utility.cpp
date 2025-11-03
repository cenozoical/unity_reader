//
// Created by vuk on 10/27/25.
//

#include "utility.h"

//for a specified delimiter, splits string into non zero sized tokens
std::vector<std::string> utilities::splitString(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        if (end != start)
            tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }
    // Add the last token
    if (start!=str.size())
        tokens.push_back(str.substr(start, end));

    return tokens;
}