#include <vector>
#include <string>
#include <iostream>
#include "utils.hpp"

std::vector<std::string> split_string(std::string str, std::string delimiter)
{
    std::vector<std::string> str_list;
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos)
    {
        token = str.substr(0, pos);
        str_list.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    str_list.push_back(str);
    return str_list;
}