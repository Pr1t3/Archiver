#include "Archiver.h"

void ConvertToBin(std::vector<bool>& a, long long num, int pos, int delta = 1) {
    while (num > 0) {
        a[pos] = num % 2;
        num /= 2;
        pos += delta;
    }
}

std::vector<bool> ToVectorBool(const char* buffer, int main_bits, int add_bits) {
    std::vector<bool> bits(main_bits + add_bits);
    for (int i = 0; i < main_bits + add_bits; ++i) {
        if (buffer[i] == '1') {
            bits[i] = true;
        }
    }
    return bits;
}

void InitializationVectorBool(std::vector<bool>& bits, int count, char sequence, int index) {
    for (int i = count; i >= 0; --i) {
        bits[index] = (sequence >> i) & 1;
        ++index;
    }
}

int GetAddBits(int main_bits) {
    int k = 0;
    while (pow(2, k) < k + main_bits + 1) {
        ++k;
    }
    return k;
}

std::vector<bool> Coder::Encode(std::vector<bool> sequence, int main_bits, int add_bits) {
    std::vector<bool> encoded_sequence(main_bits + add_bits);
    int j = 0;
    int k = 0;
    for (int i = 1; i <= main_bits + add_bits; ++i) {
        if (i == pow(2, j)) {
            encoded_sequence[i - 1] = false;
            ++j;
        } else {
            encoded_sequence[i - 1] = sequence[k];
            ++k;
        }
    }
    std::vector<bool> encoded_sequence_copy = encoded_sequence;
    for (int i = 0; i < add_bits; ++i) {
        int val = 0;
        for (int j = 1; j <= main_bits + add_bits; ++j) {
            std::vector<bool> a(add_bits);
            ConvertToBin(a, j, 0);
            val += a[i] * encoded_sequence_copy[j - 1];
        }
        encoded_sequence[pow(2, i) - 1] = val % 2;
    }

    return encoded_sequence;
}

std::vector<bool> Coder::Decode(std::vector<bool> encoded_sequence, int main_bits, int add_bits) {
    std::vector<bool> encoded_sequence_check = encoded_sequence;
    int j = 0;
    int k = 0;
    for (int i = 1; i <= main_bits + add_bits; ++i) {
        if (i == pow(2, j)) {
            encoded_sequence_check[i - 1] = false;
            ++j;
        }
    }
    int num = -1;
    for (int i = 0; i < add_bits; ++i) {
        int val = 0;
        for (int j = 1; j <= main_bits + add_bits; ++j) {
            std::vector<bool> a(add_bits);
            ConvertToBin(a, j, 0);
            val += a[i] * encoded_sequence_check[j - 1];
        }
        if (val % 2 != encoded_sequence[pow(2, i) - 1]) {
            num += pow(2, i);
        }
    }
    if (num != -1) {
        encoded_sequence[num] = !encoded_sequence[num];
    }
    std::vector<bool> sequence(main_bits);
    j = 0;
    k = 0;
    for (int i = 1; i <= main_bits + add_bits; ++i) {
        if (i != pow(2, j)) {
            sequence[k] = encoded_sequence[i - 1];
            ++k;
        } else {
            ++j;
        }
    }
    return sequence;
}

std::vector<unsigned short> Archiver::GetDateOfCreation(const std::string& file_name) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring f = converter.from_bytes(file_name);
    const wchar_t* file_name_w = f.c_str();
    WIN32_FILE_ATTRIBUTE_DATA fileAttributes;
    if (!GetFileAttributesExW(file_name_w, GetFileExInfoStandard, &fileAttributes)) {
        std::cerr << "Ошибка получения атрибутов файла: " << GetLastError() << std::endl;
        exit(EXIT_FAILURE);
    } else {
        FILETIME file_creation_time = fileAttributes.ftCreationTime;
        SYSTEMTIME system_creation_time_local, system_creation_time_UTC;

        FileTimeToSystemTime(&file_creation_time, &system_creation_time_UTC);
        SystemTimeToTzSpecificLocalTime(nullptr, &system_creation_time_UTC, &system_creation_time_local);
        unsigned short year = system_creation_time_local.wYear;
        unsigned short month = system_creation_time_local.wMonth;
        unsigned short day = system_creation_time_local.wDay;
        unsigned short hour = system_creation_time_local.wHour;
        unsigned short minute = system_creation_time_local.wMinute;
        unsigned short second = system_creation_time_local.wSecond;
        std::vector<unsigned short> data = {year, month, day, hour, minute, second};
        return data;
    }
}

void Archiver::SetDateOfCreation(std::vector<unsigned short>& date_of_creation, const std::string& file_name) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring f = converter.from_bytes(file_name);
    const wchar_t* file_name_w = f.c_str();
    FILETIME newCreateTime;
    SYSTEMTIME st;
    st.wYear = date_of_creation[0];
    st.wMonth = date_of_creation[1];
    st.wDay = date_of_creation[2];
    st.wHour = date_of_creation[3];
    st.wMinute = date_of_creation[4];
    st.wSecond = date_of_creation[5];
    SystemTimeToFileTime(&st, &newCreateTime);
    HANDLE handler_file = CreateFileW(file_name_w, FILE_WRITE_ATTRIBUTES, 0,
                       nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (handler_file == INVALID_HANDLE_VALUE) {
        std::cerr << "Ошибка открытия файла: " << GetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }
    if (!SetFileTime(handler_file, &newCreateTime, nullptr, nullptr)) {
        std::cerr << "Ошибка установки новой даты создания файла: " << GetLastError() << std::endl;
        CloseHandle(handler_file);
        exit(EXIT_FAILURE);
    }
    CloseHandle(handler_file);
}

std::vector<unsigned short> Archiver::GetDateOfLastChange(const std::string& file_name) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring f = converter.from_bytes(file_name);

    const wchar_t* file_name_w = f.c_str();
    WIN32_FILE_ATTRIBUTE_DATA fileAttributes;
    if (!GetFileAttributesExW(file_name_w, GetFileExInfoStandard, &fileAttributes)) {
        std::cerr << "Ошибка получения атрибутов файла: " << GetLastError() << std::endl;
        exit(EXIT_FAILURE);
    } else {
        FILETIME file_last_write_time = fileAttributes.ftLastWriteTime;
        SYSTEMTIME system_last_write_time;

        FileTimeToSystemTime(&file_last_write_time, &system_last_write_time);
        unsigned short year = system_last_write_time.wYear;
        unsigned short month = system_last_write_time.wMonth;
        unsigned short day = system_last_write_time.wDay;
        unsigned short hour = system_last_write_time.wHour;
        unsigned short minute = system_last_write_time.wMinute;
        unsigned short second = system_last_write_time.wSecond;
        std::vector<unsigned short> data = {year, month, day, hour, minute, second};
        return data;
    }
}

void Archiver::SetDateOfLastChange(std::vector<unsigned short>& date_of_last_change, const std::string& file_name) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring f = converter.from_bytes(file_name);
    const wchar_t* file_name_w = f.c_str();
    FILETIME newCreateTime;
    SYSTEMTIME st;
    st.wYear = date_of_last_change[0];
    st.wMonth = date_of_last_change[1];
    st.wDay = date_of_last_change[2];
    st.wHour = date_of_last_change[3];
    st.wMinute = date_of_last_change[4];
    st.wSecond = date_of_last_change[5];
    SystemTimeToFileTime(&st, &newCreateTime);
    HANDLE handler_file = CreateFileW(file_name_w, FILE_WRITE_ATTRIBUTES, 0,
                       nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (handler_file == INVALID_HANDLE_VALUE) {
        std::cerr << "Ошибка открытия файла: " << GetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }
    if (!SetFileTime(handler_file, nullptr, &newCreateTime, &newCreateTime)) {
        std::cerr << "Ошибка установки новой даты создания файла: " << GetLastError() << std::endl;
        CloseHandle(handler_file);
        exit(EXIT_FAILURE);
    }
    CloseHandle(handler_file);
}

long long Archiver::GetSize(const std::string& file_name) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring f = converter.from_bytes(file_name);

    const wchar_t* file_name_w = f.c_str();
    WIN32_FILE_ATTRIBUTE_DATA fileAttributes;
    if (!GetFileAttributesExW(file_name_w, GetFileExInfoStandard, &fileAttributes)) {
        std::cerr << "Ошибка получения атрибутов файла: " << GetLastError() << std::endl;
        exit(EXIT_FAILURE);
    } else {
        long long size = fileAttributes.nFileSizeLow;
        return size;
    }
}

std::string Archiver::GetFileName(const std::string& file_name) {
    size_t start = 0;
    for (size_t i = file_name.size() - 1; i >= 0; --i) {
        if (file_name[i] == '\\') {
            start = i;
            break;
        }
    }
    std::string name{};
    for (size_t i = start + 1; i < file_name.size(); ++i) {
        if (file_name[i] == '.') {
            break;
        }
        name.push_back(file_name[i]);
    }
    return name;
}

std::string Archiver::GetExtension(const std::string& file_name) {
    size_t start = -1;
    for (size_t i = file_name.size() - 1; i >= 0; --i) {
        if (file_name[i] == '.') {
            start = i;
            break;
        }
    }
    std::string extension{};
    if (start != -1) {
        for (size_t i = start + 1; i < file_name.size(); ++i) {
            extension.push_back(file_name[i]);
        }
    }
    return extension;
}

void Coder::Coder::EncodeSequence(std::vector<bool>& temp, std::vector<bool>& bits, int& pos_bits, std::ostream& full_name_archive, int main_bits, int add_bits) {
    int j = 0;
    while (j != temp.size()) {
        bits[pos_bits] = temp[j];
        ++pos_bits;
        ++j;
        if (pos_bits == main_bits) {
            std::vector<bool> encoded_bits = Coder::Encode(bits, main_bits, add_bits);
            for (bool bit: encoded_bits) {
                full_name_archive << bit;
            }
            pos_bits = 0;
        }
    }
}

void Coder::DecodeSequence(std::vector<bool>& temp, std::vector<bool>& bits, int& pos_bits, bool& is_ended, std::string& string, int j) {
    while (j != temp.size()) {
        bits[pos_bits] = temp[j];
        ++pos_bits;
        ++j;
        if (pos_bits == 8) {
            char sequence = 0;
            for (int m = 0; m < 8; ++m) {
                sequence |= (bits[m] << (7 - m));
            }
            if (sequence == '\a') {
                is_ended = true;
                pos_bits = 0;
                break;
            }
            string += sequence;
            pos_bits = 0;
        }
    }
}

void Coder::DecodeSequence(std::vector<bool>& temp, std::vector<bool>& bits, int& pos_bits, bool& is_ended, std::vector<unsigned short>& date, int& count, int j) {
    while (j != temp.size()) {
        bits[pos_bits] = temp[j];
        ++j;
        ++pos_bits;
        if (pos_bits == 8) {
            date[count / 2] <<= 8;
            int b = 0;
            for (int m = 0; m < 8; ++m) {
                b |= (bits[m] << (7 - m));
            }
            date[count / 2] += b;
            ++count;
            pos_bits = 0;
        }
        if (count == 12) {
            is_ended = true;
            break;
        }
    }
}

void Coder::DecodeSequence(std::vector<bool>& temp, std::vector<bool>& bits, int& pos_bits, bool& is_ended, long long& size, int& count, int j) {
    while (j != temp.size()) {
        bits[pos_bits] = temp[j];
        ++j;
        ++pos_bits;
        if (pos_bits == 8) {
            size <<= 8;
            int b = 0;
            for (int m = 0; m < 8; ++m) {
                b |= (bits[m] << (7 - m));
            }
            size += b;
            ++count;
            pos_bits = 0;
        }
        if (count == 4) {
            is_ended = true;
            break;
        }
    }
}

void Coder::DecodeSequence(std::vector<bool>& temp, std::vector<bool>& bits, int& pos_bits, bool& is_ended, std::ostream& new_file, int& count, long long size, int j) {
    while (j != temp.size()) {
        bits[pos_bits] = temp[j];
        ++j;
        ++pos_bits;
        if (pos_bits == 8) {
            char sequence = 0;
            for (int m = 0; m < 8; ++m) {
                sequence |= (bits[m] << (7 - m));
            }
            new_file << sequence;
            ++count;
            pos_bits = 0;
        }
        if (count == size) {
            is_ended = true;
            break;
        }
    }
}

void Archiver::Create(const std::string& archive_name, int main_bits) {
    std::ofstream full_name_archive(archive_name);
    if (!full_name_archive.is_open()) {
        std::cerr << "There are some problems with creating archive";
        exit(EXIT_FAILURE);
    }
    std::vector<bool> temp(32);
    ConvertToBin(temp, main_bits, 31, -1);
    std::vector<bool> bits(8);
    int pos_bits = 0;
    Coder::EncodeSequence(temp, bits, pos_bits, full_name_archive, 8, 4);
    full_name_archive.close();
}

void Archiver::Append(const std::string& archive_name, const std::string& file_name) {
    std::fstream full_name_archive(archive_name, std::ios::in | std::ios::app);
    if (!full_name_archive.is_open()) {
        std::cerr << "There are some problems with opening archive";
        exit(EXIT_FAILURE);
    }
    full_name_archive.seekg(0);
    int main_bits = 0;
    for (int i = 0; i < 4; ++i) {
        char buffer[12];
        full_name_archive.read(buffer, 12);
        std::vector<bool> temp = Coder::Decode(ToVectorBool(buffer, 8, 4), 8, 4);
        main_bits <<= 8;
        int b = 0;
        for (int m = 0; m < 8; ++m) {
            b |= (temp[m] << (7 - m));
        }
        main_bits += b;
    }
    int add_bits = GetAddBits(main_bits);
    full_name_archive.seekg(0, std::ios::end);
    std::ifstream file(file_name);
    std::string name = GetFileName(file_name);
    while (name.size() != 200) {
        name.push_back('\a');
    }
    std::string extension = GetExtension(file_name);
    while (extension.size() != 20) {
        extension.push_back('\a');
    }
    std::vector<bool> bits(main_bits);
    int pos_bits = 0;
    std::vector<unsigned short> date_of_creation = GetDateOfCreation(file_name);
    std::vector<unsigned short> date_of_last_change = GetDateOfLastChange(file_name);
    long long size = GetSize(file_name);

    for (char i: name) {
        std::vector<bool> temp(8);
        InitializationVectorBool(temp, 7, i, 0);
        Coder::EncodeSequence(temp, bits, pos_bits, full_name_archive, main_bits, add_bits);
    }
    for (char i: extension) {
        std::vector<bool> temp(8);
        InitializationVectorBool(temp, 7, i, 0);
        Coder::EncodeSequence(temp, bits, pos_bits, full_name_archive, main_bits, add_bits);
    }
    for (unsigned short i: date_of_creation) {
        std::vector<bool> temp(16);
        ConvertToBin(temp, i, 15, -1);
        Coder::EncodeSequence(temp, bits, pos_bits, full_name_archive, main_bits, add_bits);
    }
    for (unsigned short i: date_of_last_change) {
        std::vector<bool> temp(16);
        ConvertToBin(temp, i, 15, -1);
        Coder::EncodeSequence(temp, bits, pos_bits, full_name_archive, main_bits, add_bits);
    }
    std::vector<bool> temp(32);
    ConvertToBin(temp, size, 31, -1);
    Coder::EncodeSequence(temp, bits, pos_bits, full_name_archive, main_bits, add_bits);

    while (file) {
        std::vector<bool> temp(8);
        char sequence = file.get();
        if (sequence == EOF) {
            break;
        }
        InitializationVectorBool(temp, 7, sequence, 0);
        Coder::EncodeSequence(temp, bits, pos_bits, full_name_archive, main_bits, add_bits);
    }
    if (pos_bits != 0) {
        while (pos_bits != main_bits) {
            bits[pos_bits] = false;
            ++pos_bits;
        }
        std::vector<bool> encoded_bits = Coder::Encode(bits, main_bits, add_bits);
        for (bool i: encoded_bits) {
            full_name_archive << i;
        }
    }
    full_name_archive.close();
    file.close();
}

void Archiver::Extract(const std::string& archive_name, const std::string& file_name) {
    std::ifstream full_name_archive(archive_name);
    if (!full_name_archive.is_open()) {
        std::cerr << "There are some problems with opening archive";
        exit(EXIT_FAILURE);
    }
    int main_bits = 0;
    for (int i = 0; i < 4; ++i) {
        char buffer[12];
        full_name_archive.read(buffer, 12);
        std::vector<bool> temp = Coder::Decode(ToVectorBool(buffer, 8, 4), 8, 4);
        main_bits <<= 8;
        int b = 0;
        for (int m = 0; m < 8; ++m) {
            b |= (temp[m] << (7 - m));
        }
        main_bits += b;
    }
    int add_bits = GetAddBits(main_bits);
    std::string name = GetFileName(file_name);
    std::string extension = GetExtension(file_name);
    std::vector<unsigned short> date_of_creation;
    date_of_creation.resize(6);
    std::vector<unsigned short> date_of_last_change;
    date_of_last_change.resize(6);
    long long size = 0;
    std::vector<bool> bits(8);
    int pos_bits = 0;
    while (full_name_archive) {
        std::string probable_name;
        int position = 0;
        int j;
        bool is_name_ended = false;
        for (int i = 0; i < ceil(1600.0 / main_bits); ++i) {
            char buffer[main_bits + add_bits];
            full_name_archive.read(buffer, main_bits + add_bits);
            std::vector<bool> temp = Coder::Decode(ToVectorBool(buffer, main_bits, add_bits), main_bits, add_bits);
            j = 0;
            Coder::DecodeSequence(temp, bits, pos_bits, is_name_ended, probable_name, j);
            if (is_name_ended) {
                break;
            }
            ++position;
        }
        full_name_archive.seekg(-(position + 1) * (main_bits + add_bits), std::ios::cur);
        bool is_extension_ended = false;
        std::string probable_extension;
        full_name_archive.seekg(1600 / main_bits * (main_bits + add_bits), std::ios::cur);
        position = 0;
        for (int i = 0; i < ceil(160.0 / main_bits); ++i) {
            char buffer[main_bits + add_bits];
            full_name_archive.read(buffer, main_bits + add_bits);
            std::vector<bool> temp = Coder::Decode(ToVectorBool(buffer, main_bits, add_bits), main_bits, add_bits);
            i == 0 ? j = 1600 % main_bits : j = 0;
            Coder::DecodeSequence(temp, bits, pos_bits, is_extension_ended, probable_extension, j);
            if (is_extension_ended) {
                break;
            }
            ++position;
        }
        pos_bits = 0;
        if (probable_name == name && probable_extension == extension) {
            full_name_archive.seekg((160 + 1600 % main_bits) / main_bits * (main_bits + add_bits) -
                                    (position + 1) * (main_bits + add_bits), std::ios::cur);
            int count = 0;
            bool is_ended = false;
            int i = 0;
            while (i < ceil((96 + (160 + 1600 % main_bits) % main_bits) / static_cast<double>(main_bits))) {
                char buffer[main_bits + add_bits];
                full_name_archive.read(buffer, main_bits + add_bits);
                std::vector<bool> temp = Coder::Decode(ToVectorBool(buffer, main_bits, add_bits), main_bits, add_bits);
                i == 0 ? j = (160 + 1600 % main_bits) % main_bits : j = 0;
                Coder::DecodeSequence(temp, bits, pos_bits, is_ended, date_of_creation, count, j);
                ++i;
                if (is_ended) {
                    break;
                }
            }
            full_name_archive.seekg((floor((96 + (160 + 1600 % main_bits) % main_bits) / static_cast<double>(main_bits)) - i) *
                                    (main_bits + add_bits), std::ios::cur);
            is_ended = false;
            count = 0;
            i = 0;
            while (i < ceil((96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) / static_cast<double>(main_bits))) {
                char buffer[main_bits + add_bits];
                full_name_archive.read(buffer, main_bits + add_bits);
                std::vector<bool> temp = Coder::Decode(ToVectorBool(buffer, main_bits, add_bits), main_bits, add_bits);
                i == 0 ? j = (96 + (160 + 1600 % main_bits) % main_bits) % main_bits : j = 0;
                Coder::DecodeSequence(temp, bits, pos_bits, is_ended, date_of_last_change, count, j);
                ++i;
                if (is_ended) {
                    break;
                }
            }
            full_name_archive.seekg(floor(((96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) /
                                           static_cast<double>(main_bits)) - i) * (main_bits + add_bits), std::ios::cur);
            is_ended = false;
            count = 0;
            i = 0;
            size = 0;
            while (i < ceil((32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) / static_cast<double>(main_bits))) {
                char buffer[main_bits + add_bits];
                full_name_archive.read(buffer, main_bits + add_bits);
                std::vector<bool> temp = Coder::Decode(ToVectorBool(buffer, main_bits, add_bits), main_bits, add_bits);
                i == 0 ? j = (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits : j = 0;
                Coder::DecodeSequence(temp, bits, pos_bits, is_ended, size, count, j);
                ++i;
                if (is_ended) {
                    break;
                }
            }
            full_name_archive.seekg(floor(((32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) /
                                          static_cast<double>(main_bits)) - i) * (main_bits + add_bits), std::ios::cur);
            std::ofstream new_file(file_name);
            if (!new_file.is_open()) {
                std::cerr << "There is a problem with creating file";
                exit(EXIT_FAILURE);
            }
            is_ended = false;
            count = 0;
            i = 0;
            while (i < ceil((size * 8 + (32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) % main_bits) / static_cast<double>(main_bits))) {
                char buffer[main_bits + add_bits];
                full_name_archive.read(buffer, main_bits + add_bits);
                std::vector<bool> temp = Coder::Decode(ToVectorBool(buffer, main_bits, add_bits), main_bits, add_bits);
                i == 0 ? j = (32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) % main_bits : j = 0;
                Coder::DecodeSequence(temp, bits, pos_bits, is_ended, new_file, count, size, j);
                ++i;
                if (is_ended) {
                    break;
                }
            }
            SetDateOfCreation(date_of_creation, file_name);
            SetDateOfLastChange(date_of_creation, file_name);
            new_file.close();
            break;
        } else {
            full_name_archive.seekg(-(position + 1) * (main_bits + add_bits), std::ios::cur);
            full_name_archive.seekg(44 * 8 / main_bits * (main_bits + add_bits), std::ios::cur);

            bool is_ended = false;
            int count = 0;
            int i = 0;
            size = 0;
            while (i < ceil((32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) / static_cast<double>(main_bits))) {
                char buffer[main_bits + add_bits];
                full_name_archive.read(buffer, main_bits + add_bits);
                std::vector<bool> temp = Coder::Decode(ToVectorBool(buffer, main_bits, add_bits), main_bits, add_bits);
                i == 0 ? j = (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits : j = 0;
                Coder::DecodeSequence(temp, bits, pos_bits, is_ended, size, count, j);
                ++i;
                if (is_ended) {
                    break;
                }
            }

            full_name_archive.seekg((floor((size * 8 + (32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) % main_bits) /
                                           static_cast<double>(main_bits))) * (main_bits + add_bits), std::ios::cur);
            pos_bits = 0;
        }
    }

    full_name_archive.close();
}

void Archiver::Concatenate(const std::string& main_archive_name, const std::string& additional_archive_name) {
    std::fstream main_archive(main_archive_name, std::ios::in | std::ios::app);
    if (!main_archive.is_open()) {
        std::cerr << "Main archive can't be opened";
    }
    std::ifstream additional_archive(additional_archive_name);
    if (!additional_archive.is_open()) {
        std::cerr << "Additional archive can't be opened";
    }
    main_archive.seekg(0);
    int main_bits_1 = 0;
    for (int i = 0; i < 4; ++i) {
        char buffer[12];
        main_archive.read(buffer, 12);
        std::vector<bool> temp = Coder::Decode(ToVectorBool(buffer, 8, 4), 8, 4);
        main_bits_1 <<= 8;
        int b = 0;
        for (int m = 0; m < 8; ++m) {
            b |= (temp[m] << (7 - m));
        }
        main_bits_1 += b;
    }
    int main_bits_2 = 0;
    for (int i = 0; i < 4; ++i) {
        char buffer[12];
        additional_archive.read(buffer, 12);
        std::vector<bool> temp = Coder::Decode(ToVectorBool(buffer, 8, 4), 8, 4);
        main_bits_2 <<= 8;
        int b = 0;
        for (int m = 0; m < 8; ++m) {
            b |= (temp[m] << (7 - m));
        }
        main_bits_2 += b;
    }
    if (main_bits_1 != main_bits_2) {
        std::cerr << "You can't concatenate 2 archives with different encoding";
        exit(EXIT_FAILURE);
    }
    main_archive.seekg(0, std::ios::end);
    while (additional_archive) {
        char buffer[4097];
        buffer[4096] = 0;
        additional_archive.read(buffer, 4096);
        main_archive.write(buffer, additional_archive.gcount());
    }
    main_archive.close();
    additional_archive.close();
}

void Archiver::Delete(const std::string& archive_name, const std::string& file_name) {
    std::fstream full_name_archive(archive_name);
    if (!full_name_archive.is_open()) {
        std::cerr << "There is a problem with opening archive";
        exit(EXIT_FAILURE);
    }
    int main_bits = 0;
    for (int i = 0; i < 4; ++i) {
        char buffer[12];
        full_name_archive.read(buffer, 12);
        std::vector<bool> temp = Coder::Decode(ToVectorBool(buffer, 8, 4), 8, 4);
        main_bits <<= 8;
        int b = 0;
        for (int m = 0; m < 8; ++m) {
            b |= (temp[m] << (7 - m));
        }
        main_bits += b;
    }
    int add_bits = GetAddBits(main_bits);
    std::string probable_name;
    std::string name = GetFileName(file_name);
    long long size;
    std::vector<bool> bits(8);
    int pos_bits = 0;
    while (full_name_archive) {
        long long pos_low_p = full_name_archive.tellp() & MAXLONGLONG;
        long long pos_high_p = (full_name_archive.tellp() >> 32) & MAXLONGLONG;
        int position = 0;
        int j;
        bool is_name_ended = false;
        for (int i = 0; i < ceil(1600.0 / main_bits); ++i) {
            char buffer[main_bits + add_bits];
            full_name_archive.read(buffer, main_bits + add_bits);
            std::vector<bool> temp = Coder::Decode(ToVectorBool(buffer, main_bits, add_bits), main_bits, add_bits);
            j = 0;
            Coder::DecodeSequence(temp, bits, pos_bits, is_name_ended, probable_name, j);
            if (is_name_ended) {
                break;
            }
            ++position;
        }

        full_name_archive.seekg(-(position + 1) * (main_bits + add_bits), std::ios::cur);
        full_name_archive.seekg(244 * 8 / main_bits * (main_bits + add_bits), std::ios::cur);
        bool is_ended = false;
        int count = 0;
        int i = 0;
        size = 0;
        while (i < ceil((32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) / static_cast<double>(main_bits))) {
            char buffer[main_bits + add_bits];
            full_name_archive.read(buffer, main_bits + add_bits);
            std::vector<bool> temp = Coder::Decode(ToVectorBool(buffer, main_bits, add_bits), main_bits, add_bits);
            i == 0 ? j = (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits : j = 0;
            Coder::DecodeSequence(temp, bits, pos_bits, is_ended, size, count, j);
            ++i;
            if (is_ended) {
                break;
            }
        }
        if (probable_name == name) {
            full_name_archive.seekg((floor((size * 8 + (32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) % main_bits) /
                                           static_cast<double>(main_bits))) * (main_bits + add_bits), std::ios::cur);
            int cnt = 0;
            bool ok = false;
            long long pos_low_g = full_name_archive.tellg() & MAXLONGLONG;
            long long pos_high_g = full_name_archive.tellg() >> 32 & MAXLONGLONG;
            while (full_name_archive) {
                char buffer[main_bits + add_bits];
                full_name_archive.read(buffer, main_bits + add_bits);
                if (full_name_archive.tellg() != -1) {
                    pos_low_g = full_name_archive.tellg() & MAXLONGLONG;
                    pos_high_g = full_name_archive.tellg() >> 32 & MAXLONGLONG;
                }
                full_name_archive.seekp(pos_high_p << 32 | pos_low_p);
                full_name_archive.write(buffer, main_bits + add_bits);
                if (full_name_archive.tellp() != -1) {
                    pos_low_p = full_name_archive.tellp() & MAXLONGLONG;
                    pos_high_p = full_name_archive.tellp() >> 32 & MAXLONGLONG;
                }
                full_name_archive.seekg(pos_high_g << 32 | pos_low_g);
                if (cnt < size) {
                    cnt += strlen(buffer);
                } else if (!ok && cnt >= size * (main_bits + add_bits)) {
                    ok = true;
                }
            }
            if (!ok) {
                std::filesystem::resize_file(archive_name, pos_high_p << 32 | pos_low_p);
                full_name_archive.close();
                break;
            }
        } else {
            full_name_archive.seekg((floor((size * 8 + (32 +(96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) % main_bits) /
                                           static_cast<double>(main_bits))) * (main_bits + add_bits), std::ios::cur);
            probable_name.clear();
        }
    }
    full_name_archive.close();
}

void Archiver::List(const std::string& archive_name) {
    std::ifstream full_name_archive(archive_name);
    if (!full_name_archive.is_open()) {
        std::cerr << "There is a problem with opening archive";
        exit(EXIT_FAILURE);
    }
    int main_bits = 0;
    for (int i = 0; i < 4; ++i) {
        char buffer[12];
        full_name_archive.read(buffer, 12);
        std::vector<bool> temp = Coder::Decode(ToVectorBool(buffer, 8, 4), 8, 4);
        main_bits <<= 8;
        int b = 0;
        for (int m = 0; m < 8; ++m) {
            b |= (temp[m] << (7 - m));
        }
        main_bits += b;
    }
    int add_bits = GetAddBits(main_bits);
    std::vector<bool> bits(8);
    int pos_bits = 0;
    while (full_name_archive) {
        std::string name;
        int position = 0;
        int j = 0;
        bool is_name_ended = false;
        for (int i = 0; i < ceil(1600.0 / main_bits); ++i) {
            char buffer[main_bits + add_bits];
            full_name_archive.read(buffer, main_bits + add_bits);
            std::vector<bool> temp = Coder::Decode(ToVectorBool(buffer, main_bits, add_bits), main_bits, add_bits);
            j = 0;
            Coder::DecodeSequence(temp, bits, pos_bits, is_name_ended, name, j);
            if (is_name_ended) {
                break;
            }
            ++position;
        }
        std::cout << name << '\n';
        full_name_archive.seekg(-(position + 1) * (main_bits + add_bits), std::ios::cur);
        full_name_archive.seekg(244 * 8 / main_bits * (main_bits + add_bits), std::ios::cur);
        bool is_ended = false;
        int count = 0;
        int i = 0;
        long long size = 0;
        while (i < ceil((32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) / static_cast<double>(main_bits))) {
            char buffer[main_bits + add_bits];
            full_name_archive.read(buffer, main_bits + add_bits);
            std::vector<bool> temp = Coder::Decode(ToVectorBool(buffer, main_bits, add_bits), main_bits, add_bits);
            i == 0 ? j = (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits : j = 0;
            Coder::DecodeSequence(temp, bits, pos_bits, is_ended, size, count, j);
            ++i;
            if (is_ended) {
                break;
            }
        }
        full_name_archive.seekg((floor((size * 8 + (32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) % main_bits) /
                                       static_cast<double>(main_bits))) * (main_bits + add_bits), std::ios::cur);
        long long pos_low_g = full_name_archive.tellg() & MAXLONGLONG;
        long long pos_high_g = full_name_archive.tellg() >> 32 & MAXLONGLONG;
        full_name_archive.seekg(0, std::ios::end);
        if (((pos_high_g << 32) | pos_low_g) >= full_name_archive.tellg() || pos_low_g == -1) {
            full_name_archive.close();
            break;
        } else {
            full_name_archive.seekg((pos_high_g << 32) | pos_low_g, std::ios::beg);
        }
    }
}