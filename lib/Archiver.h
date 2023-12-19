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

class Coder {
public:
    static void EncodeSequence(std::vector<bool>& temp, std::vector<bool>& bits, int& pos_bits, std::ostream& full_name_archive, int main_bits, int add_bits);

    static void DecodeSequence(std::vector<bool>& temp, std::vector<bool>& bits, int& pos_bits, bool& is_ended, std::string& string, int j);

    static void DecodeSequence(std::vector<bool>& temp, std::vector<bool>& bits, int& pos_bits, bool& is_ended, std::vector<unsigned short>& date, int& count, int j);

    static void DecodeSequence(std::vector<bool>& temp, std::vector<bool>& bits, int& pos_bits, bool& is_ended, long long& size, int& count, int j);

    static void DecodeSequence(std::vector<bool>& temp, std::vector<bool>& bits, int& pos_bits, bool& is_ended, std::ostream& new_file, int& count, long long size, int j);

    static std::vector<bool> Encode(std::vector<bool> symbol, int main_bits, int add_bits);

    static std::vector<bool> Decode(std::vector<bool> encoded_symbol, int main_bits, int add_bits);
};

class Archiver {
private:
    static std::string GetFileName(const std::string& file_name);

    static std::string GetExtension(const std::string& file_name);

    static std::vector<unsigned short> GetDateOfCreation(const std::string& file_name);

    static void SetDateOfCreation(std::vector<unsigned short>& date_of_creation, const std::string& file_name);

    static std::vector<unsigned short> GetDateOfLastChange(const std::string& file_name);

    static void SetDateOfLastChange(std::vector<unsigned short>& date_of_last_change, const std::string& file_name);

    static long long GetSize(const std::string& file_name);

public:
    static void Create(const std::string& archive_name, int main_bits = 8);

    static void Append(const std::string& archive_name, const std::string& file_name);

    static void Extract(const std::string& archive_name, const std::string& file_name);

    static void List(const std::string& archive_name);

    static void Delete(const std::string& archive_name, const std::string& file_name);

    static void Concatenate(const std::string& main_archive, const std::string& additional_archive);
};