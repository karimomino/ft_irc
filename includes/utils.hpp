#pragma once

#include <iostream>
#include <vector>
#include <sstream>

namespace utils {
void trim( std::string& str );
std::vector<std::string> split( std::string str , std::string delm);
template <typename T> std::string numToA(T num){
    std::stringstream ss;

    ss << num;
    return (ss.str());
};
}
