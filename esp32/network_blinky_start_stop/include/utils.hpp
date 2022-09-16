#pragma once

#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <array>
#include <sstream>
  
inline std::string ip_to_hex_string(int addr)
{
    // convert to hex string
    char str[15];  
    sprintf(str, "0x%08x", addr);
    return str;
}

inline std::string get_reversed_hex_ip_addr(std::string& ip_str) {
    std::string tmp_string = ip_str;
    int addr = inet_addr(ip_str.c_str());  // build in C
    return ip_to_hex_string(addr);
}