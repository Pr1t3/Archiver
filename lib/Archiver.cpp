#include "Archiver.h"

void Bin(std::vector<bool>& a, int num, int pos, int delta=1){
    while(num > 0){
        a[pos] = num % 2;
        num /= 2;
        pos += delta;
    }
}

std::vector<bool> Archiver::Encode(std::vector<bool> symbol) {
    std::vector<bool> encoded_symbol(main_bits+add_bits);
    int j = 0;
    int k = 0;
    for(int i = 1; i <= main_bits+add_bits; ++i){
        if(i == pow(2, j)){
            encoded_symbol[i-1] = false;
            ++j;
        } else{
            encoded_symbol[i-1] = symbol[k];
            ++k;
        }
    }
    std::vector<bool> encoded_symbol_copy = encoded_symbol;
    for(int i = 0; i < add_bits; ++i) {
        int val = 0;
        for (int j = 1; j <= main_bits+add_bits; ++j) {
            std::vector<bool> a(add_bits);
            Bin(a, j, 0);
            val += a[i]*encoded_symbol_copy[j-1];
        }
        encoded_symbol[pow(2, i)-1] = val % 2;
    }

    return encoded_symbol;
}

std::vector<bool> Archiver::Decode(std::vector<bool> encoded_symbol) {
    // Добавить функцию проверки на 2 ошибки, если ошибки не 2, то перейти сюда и если надо, то исправить 1 бит
    std::vector<bool> symbol(main_bits);
    int j = 0;
    int k = 0;
    for(int i = 1; i <= main_bits+add_bits; ++i){
        if(i != pow(2, j)){
            symbol[k] = encoded_symbol[i-1];
            ++k;
        } else {
            ++j;
        }
    }
    return symbol;
}

std::vector<bool> ToVectorBool(char* buffer){
    std::vector<bool> bits(Archiver::main_bits+Archiver::add_bits);
    for(int i = 0; i < Archiver::main_bits+Archiver::add_bits; ++i){
        if(buffer[i] == '1'){
            bits[i] = true;
        }
    }
    return bits;
}

std::vector<unsigned short> Archiver::GetDateOfCreation(std::string file_name) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring f = converter.from_bytes(file_name);

    const wchar_t* file_name_w = f.c_str();
    WIN32_FILE_ATTRIBUTE_DATA fileAttributes;
    if (GetFileAttributesExW(file_name_w, GetFileExInfoStandard, &fileAttributes)) {
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
    } else {
        std::cerr << "Ошибка получения атрибутов файла: " << GetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Archiver::SetDateOfCreation(std::vector<unsigned short>& date_of_creation, std::string file_name) {
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
std::vector<unsigned short> Archiver::GetDateOfLastChange(std::string file_name) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring f = converter.from_bytes(file_name);

    const wchar_t* file_name_w = f.c_str();
    WIN32_FILE_ATTRIBUTE_DATA fileAttributes;
    if (GetFileAttributesExW(file_name_w, GetFileExInfoStandard, &fileAttributes)) {
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
    } else {
        std::cerr << "Ошибка получения атрибутов файла: " << GetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Archiver::SetDateOfLastChange(std::vector<unsigned short>& date_of_last_change, std::string file_name) {
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

long long Archiver::GetSize(std::string file_name) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring f = converter.from_bytes(file_name);

    const wchar_t* file_name_w = f.c_str();
    WIN32_FILE_ATTRIBUTE_DATA fileAttributes;
    if (GetFileAttributesExW(file_name_w, GetFileExInfoStandard, &fileAttributes)) {
        long long size = fileAttributes.nFileSizeLow;

        return size;
    } else {
        std::cerr << "Ошибка получения атрибутов файла: " << GetLastError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::string Archiver::GetFileName(std::string file_name) {
    size_t start = 0;
    for(size_t i = file_name.size()-1; i >= 0; --i){
        if(file_name[i] == '\\'){
            start = i;
            break;
        }
    }
    std::string name{};
    for(size_t i = start+1; i < file_name.size(); ++i){
        if(file_name[i] == '.'){
            break;
        }
        name.push_back(file_name[i]);
    }
    return name;
}

std::string Archiver::GetExtension(std::string file_name) {
    size_t start = -1;
    for(size_t i = file_name.size()-1; i >= 0; --i){
        if(file_name[i] == '.'){
            start = i;
            break;
        }
    }
    std::string extension{};
    if(start != -1) {
        for (size_t i = start+1; i < file_name.size(); ++i) {
            extension.push_back(file_name[i]);
        }
    }
    return extension;
}

void Archiver::Create(std::string archive_name) {
    archive_name += ".haf";
    std::ofstream full_name_archive(archive_name);
    if(!full_name_archive.is_open()){
        std::cerr << "There are some problems with creating archive";
        exit(EXIT_FAILURE);
    }
    full_name_archive.close();
}

void InitializationVectorBool(std::vector<bool>& bits, int count, char symbol, int index){
    for(int i = count; i >= 0; --i){
        bits[index] = (symbol >> i) & 1;
        ++index;
    }
}

void Archiver::Append(std::string archive_name, std::string file_name) {
    std::fstream full_name_archive(archive_name, std::ios::app);
    if(!full_name_archive.is_open()){
        std::cerr << "There are some problems with opening archive";
        exit(EXIT_FAILURE);
    }
    std::ifstream file(file_name);
    std::string name = GetFileName(file_name);
    while(name.size() != 200){
        name.push_back('\a');
    }
    std::string extension = GetExtension(file_name);
    while(extension.size() != 20){
        extension.push_back('\a');
    }
    std::vector<unsigned short> date_of_creation = GetDateOfCreation(file_name);
    std::vector<unsigned short> date_of_last_change = GetDateOfLastChange(file_name);
    long long size = GetSize(file_name);
    std::vector<bool> bits(main_bits);
    int pos_bits = 0;

    for(char i: name){
        std::vector<bool> temp(8);
        InitializationVectorBool(temp, 7, i, 0);
        int j = 0;
        while(j != temp.size()){
            bits[pos_bits] = temp[j];
            ++pos_bits;
            ++j;
            if(pos_bits == main_bits) {
                std::vector<bool> encoded_bits = Encode(bits);
                //001100010001011
                for (bool i: encoded_bits) {
                    full_name_archive << i;
                }
                pos_bits = 0;
            }
        }
    }

    for(char i: extension){
        std::vector<bool> temp(8);
        InitializationVectorBool(temp, 7, i, 0);
        int j = 0;
        while(pos_bits + temp.size()-j >= main_bits){
            while(pos_bits != main_bits){
                bits[pos_bits] = temp[j];
                ++pos_bits;
                ++j;
            }
            std::vector<bool> encoded_bits = Encode(bits);
            for(bool i: encoded_bits){
                full_name_archive << i;
            }
            pos_bits = 0;
        }
        while(j != temp.size()){
            bits[pos_bits] = temp[j];
            ++pos_bits;
            ++j;
        }
    }

    for(unsigned short i: date_of_creation){
        std::vector<bool> temp(16);
        Bin(temp, i, 15, -1);
        int j = 0;
        while(pos_bits + temp.size()-j >= main_bits){
            while(pos_bits != main_bits){
                bits[pos_bits] = temp[j];
                ++pos_bits;
                ++j;
            }
            std::vector<bool> encoded_bits = Encode(bits);
            for(bool m: encoded_bits){
                full_name_archive << m;
            }
            pos_bits = 0;
        }
        while(j != temp.size()){
            bits[pos_bits] = temp[j];
            ++pos_bits;
            ++j;
        }
    }

    for(unsigned short i: date_of_last_change){
        std::vector<bool> temp(16);
        Bin(temp, i, 15, -1);
        int j = 0;
        while(pos_bits + temp.size()-j >= main_bits){
            while(pos_bits != main_bits){
                bits[pos_bits] = temp[j];
                ++pos_bits;
                ++j;
            }
            std::vector<bool> encoded_bits = Encode(bits);
            for(bool i: encoded_bits){
                full_name_archive << i;
            }
            pos_bits = 0;
        }
        while(j != temp.size()){
            bits[pos_bits] = temp[j];
            ++pos_bits;
            ++j;
        }
    }

    std::vector<bool> temp(32);
    Bin(temp, size, 31, -1);
    int j = 0;
    while(pos_bits + temp.size()-j >= main_bits){
        while(pos_bits != main_bits){
            bits[pos_bits] = temp[j];
            ++pos_bits;
            ++j;
        }
        std::vector<bool> encoded_bits = Encode(bits);
        for(bool i: encoded_bits){
            full_name_archive << i;
        }
        pos_bits = 0;
    }

    while(j != temp.size()){
        bits[pos_bits] = temp[j];
        ++pos_bits;
        ++j;
    }

    while(file){
        std::vector<bool> temp(8);
        char symbol = file.get();
        if(symbol == EOF){
            break;
        }
        InitializationVectorBool(temp, 7, symbol, 0);
        int j = 0;
        while(pos_bits + temp.size()-j >= main_bits){
            while(pos_bits != main_bits){
                bits[pos_bits] = temp[j];
                ++pos_bits;
                ++j;
            }
            std::vector<bool> encoded_bits = Encode(bits);
            for(bool i: encoded_bits){
                full_name_archive << i;
            }
            pos_bits = 0;
        }
        while(j != temp.size()){
            bits[pos_bits] = temp[j];
            ++pos_bits;
            ++j;
        }
    }
    if(pos_bits != 0){
        while(pos_bits != main_bits){
            bits[pos_bits] = false;
            ++pos_bits;
        }
        std::vector<bool> encoded_bits = Encode(bits);
        for(bool i: encoded_bits){
            full_name_archive << i;
        }
    }
    full_name_archive.close();
    file.close();
}

void Archiver::Extract(std::string archive_name, std::string file_name) {
    std::ifstream full_name_archive(archive_name);
    if(!full_name_archive.is_open()){
        std::cerr << "There are some problems with opening archive";
        exit(EXIT_FAILURE);
    }
    std::string name = GetFileName(file_name);
    std::string probable_name;
    std::string extension;
    std::vector<unsigned short> date_of_creation;
    date_of_creation.resize(6);
    std::vector<unsigned short> date_of_last_change;
    date_of_last_change.resize(6);
    long long size = 0;
    std::vector<bool> bits(8);
    int pos_bits = 0;
    while(full_name_archive) {
        int position = 0;
        int j = 0;
        bool is_name_ended = false;
        for (int i = 0; i < ceil(1600.0 / main_bits); ++i) {
            char buffer[main_bits + add_bits];
            full_name_archive.read(buffer, main_bits + add_bits);
            std::vector<bool> temp = Decode(ToVectorBool(buffer));
            j = 0;
            while (j != temp.size()) {
                bits[pos_bits] = temp[j];
                ++pos_bits;
                ++j;
                if (pos_bits == 8) {
                    char symbol = 0;
                    for (int m = 0; m < 8; ++m) {
                        symbol |= (bits[m] << (7 - m));
                    }
                    if (symbol == '\a') {
                        is_name_ended = true;
                        pos_bits = 0;
                        break;
                    }
                    probable_name += symbol;
                    pos_bits = 0;
                }
            }
            if (is_name_ended) {
                break;
            }
            ++position;
        }
        //full_name_archive.seekg(1600/main_bits * (main_bits+add_bits) - full_name_archive.tellg());
        /*if (!is_name_ended) {
            int count = 0;
            while (count < 1600 % main_bits) {
                char buffer[main_bits + add_bits];
                full_name_archive.read(buffer, main_bits + add_bits);
                std::vector<bool> temp = Decode(ToVectorBool(buffer));
                j = 0;
                while (j != temp.size()) {
                    bits[pos_bits] = temp[j];
                    ++j;
                    ++pos_bits;
                    if (pos_bits == 8) {
                        char symbol = 0;
                        for (int m = 0; m < 8; ++m) {
                            symbol |= (bits[m] << (7 - m));
                        }
                        if (symbol == '\a') {
                            is_name_ended = true;
                            pos_bits = 0;
                            break;
                        }
                        probable_name += symbol;
                        pos_bits = 0;
                    }
                }
                if (is_name_ended) {
                    break;
                }
                count += (main_bits + add_bits);
                ++position;
            }
        }
        pos_bits = 0;
        */
        full_name_archive.seekg(-(position + 1) * (main_bits + add_bits), std::ios::cur);
        bool is_extension_ended = false;
        if (probable_name == name) {
            full_name_archive.seekg(1600 / main_bits * (main_bits + add_bits), std::ios::cur);
            position = 0;
            for (int i = 0; i < ceil(160.0 / main_bits); ++i) {
                char buffer[main_bits + add_bits];
                full_name_archive.read(buffer, main_bits + add_bits);
                std::vector<bool> temp = Decode(ToVectorBool(buffer));
                i == 0 ? j = 1600 % main_bits : j = 0;
                while (j != temp.size()) {
                    bits[pos_bits] = temp[j];
                    ++pos_bits;
                    ++j;
                    if (pos_bits == 8) {
                        char symbol = 0;
                        for (int m = 0; m < 8; ++m) {
                            symbol |= (bits[m] << (7 - m));
                        }
                        if (symbol == '\a') {
                            is_extension_ended = true;
                            pos_bits = 0;
                            break;
                        }
                        extension += symbol;
                        pos_bits = 0;
                    }
                }
                if (is_extension_ended) {
                    break;
                }
                ++position;
            }
            /*
            if (!is_extension_ended) {
                int count = 0;
                while (count < 1600 % main_bits) {
                    char buffer[main_bits + add_bits];
                    full_name_archive.read(buffer, main_bits + add_bits);
                    std::vector<bool> temp = Decode(ToVectorBool(buffer));
                    count == 0 ? j = 1600 % main_bits : j = 0;
                    while (j != temp.size()) {
                        bits[pos_bits] = temp[j];
                        ++pos_bits;
                        ++j;
                        if (pos_bits == 8) {
                            char symbol = 0;
                            for (int m = 0; m < 8; ++m) {
                                symbol |= (bits[m] << (7 - m));
                            }
                            if (symbol == '\a') {
                                is_extension_ended = true;
                                pos_bits = 0;
                                break;
                            }
                            extension += symbol;
                            pos_bits = 0;
                        }
                    }
                    if (is_extension_ended) {
                        break;
                    }
                    count += (main_bits + add_bits);
                    ++position;
                }
            }
            */
            pos_bits = 0;
            full_name_archive.seekg((160 + 1600 % main_bits) / main_bits * (main_bits + add_bits) -
                                    (position + 1) * (main_bits + add_bits), std::ios::cur);
            int d = full_name_archive.tellg();

            int count = 0;
            bool is_ended = false;
            int i = 0;
            while (i < ceil((96 + (160 + 1600 % main_bits) % main_bits) / static_cast<double>(main_bits))) {
                char buffer[main_bits + add_bits];
                full_name_archive.read(buffer, main_bits + add_bits);
                std::vector<bool> temp = Decode(ToVectorBool(buffer));
                i == 0 ? j = (160 + 1600 % main_bits) % main_bits : j = 0;
                while (j != temp.size()) {
                    bits[pos_bits] = temp[j];
                    ++j;
                    ++pos_bits;
                    if (pos_bits == 8) {
                        date_of_creation[count / 2] <<= 8;
                        int b = 0;
                        for (int m = 0; m < 8; ++m) {
                            b |= (bits[m] << (7 - m));
                        }
                        date_of_creation[count / 2] += b;
                        ++count;
                        pos_bits = 0;
                    }
                    if (count == 12) {
                        is_ended = true;
                        break;
                    }
                }
                ++i;
                if (is_ended) {
                    break;
                }
            }
            //int offset;
            //ceil(96/ static_cast<double>(main_bits)) == 96/static_cast<double>(main_bits) ? offset = 0 : offset = 1;
            full_name_archive.seekg(
                    (floor((96 + (160 + 1600 % main_bits) % main_bits) / static_cast<double>(main_bits)) - i) *
                    (main_bits + add_bits), std::ios::cur);
            d = full_name_archive.tellg();

            is_ended = false;
            count = 0;
            i = 0;
            while (i < ceil((96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) /
                            static_cast<double>(main_bits))) {
                char buffer[main_bits + add_bits];
                full_name_archive.read(buffer, main_bits + add_bits);
                std::vector<bool> temp = Decode(ToVectorBool(buffer));
                i == 0 ? j = (96 + (160 + 1600 % main_bits) % main_bits) % main_bits : j = 0;
                while (j != temp.size()) {
                    bits[pos_bits] = temp[j];
                    ++j;
                    ++pos_bits;
                    if (pos_bits == 8) {
                        date_of_last_change[count / 2] <<= 8;
                        int b = 0;
                        for (int m = 0; m < 8; ++m) {
                            b |= (bits[m] << (7 - m));
                        }
                        date_of_last_change[count / 2] += b;
                        ++count;
                        pos_bits = 0;
                    }
                    if (count == 12) {
                        is_ended = true;
                        break;
                    }
                }
                ++i;
                if (is_ended) {
                    break;
                }
            }
            //ceil(96/static_cast<double>(main_bits)) == 96/static_cast<double>(main_bits) ? offset = 0 : offset = -1;
            full_name_archive.seekg(floor(((96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) /
                                           static_cast<double>(main_bits)) - i) * (main_bits + add_bits),
                                    std::ios::cur);

            is_ended = false;
            count = 0;
            i = 0;
            size = 0;
            while (i < ceil((32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) /
                            static_cast<double>(main_bits))) {
                char buffer[main_bits + add_bits];
                full_name_archive.read(buffer, main_bits + add_bits);
                std::vector<bool> temp = Decode(ToVectorBool(buffer));
                i == 0 ? j = (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits : j = 0;
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
                ++i;
                if (is_ended) {
                    break;
                }
            }
            full_name_archive.seekg(
                    floor(((32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) /
                           static_cast<double>(main_bits)) - i) * (main_bits + add_bits), std::ios::cur);

            std::ofstream new_file(file_name);
            if (!new_file.is_open()) {
                std::cerr << "There is a problem with creating file";
                exit(EXIT_FAILURE);
            }
            is_ended = false;
            count = 0;
            i = 0;
            while (i < ceil((size * 8 +
                             (32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) %
                             main_bits) / static_cast<double>(main_bits))) {
                char buffer[main_bits + add_bits];
                full_name_archive.read(buffer, main_bits + add_bits);
                std::vector<bool> temp = Decode(ToVectorBool(buffer));
                i == 0 ? j = (32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) %
                             main_bits : j = 0;
                while (j != temp.size()) {
                    bits[pos_bits] = temp[j];
                    ++j;
                    ++pos_bits;
                    if (pos_bits == 8) {
                        char symbol = 0;
                        for (int m = 0; m < 8; ++m) {
                            symbol |= (bits[m] << (7 - m));
                        }
                        new_file << symbol;
                        ++count;
                        pos_bits = 0;
                    }
                    if (count == size) {
                        is_ended = true;
                        break;
                    }
                }
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
            full_name_archive.seekg(ceil(244*8/main_bits*(main_bits+add_bits)), std::ios::cur);

            bool is_ended = false;
            int count = 0;
            int i = 0;
            size = 0;
            while (i < ceil((32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) /
                            static_cast<double>(main_bits))) {
                char buffer[main_bits + add_bits];
                full_name_archive.read(buffer, main_bits + add_bits);
                std::vector<bool> temp = Decode(ToVectorBool(buffer));
                i == 0 ? j = (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits : j = 0;
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
                ++i;
                if (is_ended) {
                    break;
                }
            }

            full_name_archive.seekg((floor((size * 8 + (32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) % main_bits)/
                                           static_cast<double>(main_bits)))*(main_bits+add_bits), std::ios::cur);

            probable_name.clear();
            pos_bits = 0;
        }
    }

    full_name_archive.close();
}

void Archiver::Concatenate(std::string main_archive_name, std::string additional_archive_name) {
    std::fstream main_archive(main_archive_name, std::ios::app);
    if(!main_archive.is_open()){
        std::cerr << "Main archive can't be opened";
    }
    std::fstream additional_archive(additional_archive_name);
    if(!additional_archive.is_open()){
        std::cerr << "Additional archive can't be opened";
    }
    char buffer[4096];
    while(additional_archive){
        additional_archive.read(buffer, 4096);
        main_archive.write(buffer, strlen(buffer));
    }
    main_archive.close();
    additional_archive.close();
}

void Archiver::Delete(std::string archive_name, std::string file_name) {
    std::fstream full_name_archive(archive_name, std::ios::in | std::ios::out | std::ios::binary);
    if(!full_name_archive.is_open()){
        std::cerr << "There is a problem with opening archive";
        exit(EXIT_FAILURE);
    }
    std::string probable_name;
    std::string name = GetFileName(file_name);
    int size;
    std::vector<bool> bits(8);
    int pos_bits = 0;
    while(full_name_archive){
        int position = 0;
        int j = 0;
        bool is_name_ended = false;
        for (int i = 0; i < ceil(1600.0 / main_bits); ++i) {
            char buffer[main_bits + add_bits];
            full_name_archive.read(buffer, main_bits + add_bits);
            std::vector<bool> temp = Decode(ToVectorBool(buffer));
            j = 0;
            while (j != temp.size()) {
                bits[pos_bits] = temp[j];
                ++pos_bits;
                ++j;
                if (pos_bits == 8) {
                    char symbol = 0;
                    for (int m = 0; m < 8; ++m) {
                        symbol |= (bits[m] << (7 - m));
                    }
                    if (symbol == '\a') {
                        is_name_ended = true;
                        pos_bits = 0;
                        break;
                    }
                    probable_name += symbol;
                    pos_bits = 0;
                }
            }
            if (is_name_ended) {
                break;
            }
            ++position;
        }
        if(probable_name == name){
            full_name_archive.seekg(-(position + 1) * (main_bits + add_bits), std::ios::cur);
            long long pos_low_p = full_name_archive.tellp() & MAXLONGLONG - (position+1)*12;
            long long pos_high_p = (full_name_archive.tellp() >> 32) & MAXLONGLONG;
            full_name_archive.seekg(1600 / main_bits * (main_bits + add_bits), std::ios::cur);
            full_name_archive.seekg(ceil(244*8/main_bits*(main_bits+add_bits)), std::ios::cur);
            bool is_ended = false;
            int count = 0;
            int i = 0;
            size = 0;
            while (i < ceil((32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) /
                            static_cast<double>(main_bits))) {
                char buffer[main_bits + add_bits];
                full_name_archive.read(buffer, main_bits + add_bits);
                std::vector<bool> temp = Decode(ToVectorBool(buffer));
                i == 0 ? j = (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits : j = 0;
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
                ++i;
                if (is_ended) {
                    break;
                }
            }
            full_name_archive.seekg((floor((size * 8 + (32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) % main_bits)/
                                           static_cast<double>(main_bits)))*(main_bits+add_bits), std::ios::cur);
            int cnt = 0;
            bool ok = false;
            long long pos_low_g = full_name_archive.tellg() & MAXLONGLONG;
            long long pos_high_g = full_name_archive.tellg() >> 32 & MAXLONGLONG;
            while(full_name_archive){
                char buffer[main_bits+add_bits];
                full_name_archive.read(buffer, main_bits+add_bits);
                //buffer[12] = '\0';
                if(full_name_archive.tellg() != -1) {
                    pos_low_g = full_name_archive.tellg() & MAXLONGLONG;
                    pos_high_g = full_name_archive.tellg() >> 32 & MAXLONGLONG;
                }
                full_name_archive.seekp(pos_high_p << 32 | pos_low_p);
                full_name_archive.write(buffer, main_bits+add_bits);
                if(full_name_archive.tellp() != -1) {
                    pos_low_p = full_name_archive.tellp() & MAXLONGLONG;
                    pos_high_p = full_name_archive.tellp() >> 32 & MAXLONGLONG;
                }
                full_name_archive.seekg(pos_high_g << 32 | pos_low_g);
                if(cnt < size) {
                    cnt += strlen(buffer);
                } else if (!ok && cnt >= size*(main_bits+add_bits)){
                    ok = true;
                }
            }
            if(!ok){
                std::filesystem::resize_file(archive_name, pos_high_p << 32 | pos_low_p);
                full_name_archive.close();
                break;
            }
        } else {
            full_name_archive.seekg(-(position + 1) * (main_bits + add_bits), std::ios::cur);
            full_name_archive.seekg(1600 / main_bits * (main_bits + add_bits), std::ios::cur);
            full_name_archive.seekg(ceil(244*8/main_bits*(main_bits+add_bits)), std::ios::cur);
            bool is_ended = false;
            int count = 0;
            int i = 0;
            size = 0;
            while (i < ceil((32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) /
                            static_cast<double>(main_bits))) {
                char buffer[main_bits + add_bits];
                full_name_archive.read(buffer, main_bits + add_bits);
                std::vector<bool> temp = Decode(ToVectorBool(buffer));
                i == 0 ? j = (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits : j = 0;
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
                ++i;
                if (is_ended) {
                    break;
                }
            }
            full_name_archive.seekg((floor((size * 8 + (32 + (96 + (96 + (160 + 1600 % main_bits) % main_bits) % main_bits) % main_bits) % main_bits)/
                                           static_cast<double>(main_bits)))*(main_bits+add_bits), std::ios::cur);
            probable_name.clear();
        }
    }
    full_name_archive.close();
}
/*
void Archiver::List(std::string archive_name) {
    std::fstream full_name_archive(archive_name, std::ios::in | std::ios::out | std::ios::binary);
    if(!full_name_archive.is_open()){
        std::cerr << "There is a problem with opening archive";
        exit(EXIT_FAILURE);
    }
    while(full_name_archive){
        std::string name;
        int position = 0;
        for(int i = 0; i < 200; ++i) {
            char buffer[12];
            full_name_archive.read(buffer, 12);
            char decoded_symbol = static_cast<char>(Decode(ToBitset(buffer)));
            if(decoded_symbol == '\a'){
                break;
            }
            name += decoded_symbol;
            ++position;
        }
        std::cout << name << '\n'; // можно добавить расширения файлов
        full_name_archive.seekg((200-position-1+20+12+12)*12, std::ios::cur);
        int size;
        for(int i = 0; i < 4; ++i){
            char buffer[12];
            full_name_archive.read(buffer, 12);
            size <<= 8;
            size |= Decode(ToBitset(buffer));
        }
        full_name_archive.seekg(size*12, std::ios::cur);
    }
}
*/

//01100001011 00010011000 11011001000 1100101 ||| 01100110011 00111011010 00