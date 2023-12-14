#pragma once
#include <iostream>
#include <fstream>
#include <bitset>
#include <cmath>
#include <filesystem>
#include <chrono>
#include <ctime>
#include <windows.h>
#include <vector>
#include <unistd.h>
#include <timezoneapi.h>

class Archiver{
public:
    const static int main_bits = 1700; // temporary
    const static int add_bits = 11; // temporary
    std::vector<bool> Encode(std::vector<bool> symbol);
    std::vector<bool> Decode(std::vector<bool> encoded_symbol);
    std::string GetFileName(std::string file_name);
    std::string GetExtension(std::string file_name);
    std::vector<unsigned short> GetDateOfCreation(std::string file_name);
    void SetDateOfCreation(std::vector<unsigned short>& date_of_creation, std::string file_name);
    std::vector<unsigned short> GetDateOfLastChange(std::string file_name);
    void SetDateOfLastChange(std::vector<unsigned short>& date_of_last_change, std::string file_name);
    long long GetSize(std::string file_name);
public:
    void Create(std::string archive_name);
    void Append(std::string archive_name, std::string file_name);
    void Extract(std::string archive_name, std::string file_name);
    void List(std::string archive_name);
    void Delete(std::string archive_name, std::string file_name);
    void Concatenate(std::string main_archive, std::string additional_archive);
};