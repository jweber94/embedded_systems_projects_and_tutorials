#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include <array>
#include <sstream>
  
std::string ip_to_hex_string(int addr)
{
    // convert to hex string
    char str[15];  
    sprintf(str, "0x%08x", addr);
    return std::move(str);
}

std::string get_reversed_hex_ip_addr(std::string& ip_str) {
    int addr = inet_addr(ip_str.c_str());  // build in C
    return ip_to_hex_string(addr);
}

int main() {
    std::string test_ip = "192.168.2.110";
    std::string result = get_reversed_hex_ip_addr(test_ip);
    std::cout << "Result is: " << result << "\n"; 
	return 0;
}
