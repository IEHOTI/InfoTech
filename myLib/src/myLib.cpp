#include "myLib.hpp"

void sortString(std::string& str) {

	std::sort(str.begin(), str.end(), std::greater<char>());

    std::string result;
    for (char c : str) {
        if ((c-'0') % 2 == 0) {
            result += "КВ";
        }
        else {
            result += c;
        }
    }
    str.clear();
    str = result;
}

std::string sumString(std::string& str) {
    int count = 0;
    for (const char& x : str) {
        if (x >= '0' && x <= '9') count += x - '0';
    }
    return std::to_string(count);
}

bool analyzeData(std::string& str) {
    if (str.size() > 2){
        int x = std::stoi(str);
        if (x % 32 == 0) return true;
    }
    return false;
}
