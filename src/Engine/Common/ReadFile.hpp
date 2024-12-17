#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "log.hpp"

#define Real float

namespace TourBillon
{
    std::vector<char> readFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);
    
        if (!file.is_open()) {
            LOG_WARNING("failed to open file!");
            return std::vector<char>();
        }
    
        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);
    
        file.seekg(0);
        file.read(buffer.data(), fileSize);
    
        file.close();
    
        return buffer;
    }

}