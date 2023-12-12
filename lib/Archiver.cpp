#include "Archiver.h"

std::bitset<12> Archiver::Encode(std::bitset<8> symbol) {
    for(int i = 0; i < 4; ++i){
        bool k = symbol[i];
        symbol[i] = symbol[7-i];
        symbol[7-i] = k;
    }
    std::bitset<12> encoded_symbol;
    int j = 0;
    int k = 0;
    for(int i = 1; i <= 12; ++i){
        if(i == pow(2, j)){
            encoded_symbol[i-1] = false;
            ++j;
        } else{
            encoded_symbol[i-1] = symbol[k];
            ++k;
        }
    }
    std::bitset<12> encoded_symbol_copy = encoded_symbol;
    for(int i = 0; i < 4; ++i) {
        int val = 0;
        for (int j = 1; j <= 12; ++j) {
            std::bitset<4> a = j;
            val += a[i]*encoded_symbol_copy[j-1];
        }
        encoded_symbol[pow(2, i)-1] = val % 2;
    }
    for(int i = 0; i < 6; ++i){
        bool k = encoded_symbol[i];
        encoded_symbol[i] = encoded_symbol[11-i];
        encoded_symbol[11-i] = k;
    }

    return encoded_symbol;
}

int Archiver::Decode(std::bitset<12> encoded_symbol) {
    // Добавить функцию проверки на 2 ошибки, если ошибки не 2, то перейти сюда и если надо, то исправить 1 бит
    std::bitset<8> symbol;
    int j = 0;
    int k = 0;
    for(int i = 1; i <= 12; ++i){
        if(i != pow(2, j)){
            symbol[k] = encoded_symbol[i-1];
            ++k;
        } else {
            ++j;
        }
    }
    for(int i = 0; i < 4; ++i){
        bool k = symbol[i];
        symbol[i] = symbol[7-i];
        symbol[7-i] = k;
    }

    return static_cast<int>(symbol.to_ulong());
}

std::bitset<12> ToBitset(char* buffer){
    std::bitset<12> bits;
    for(int i = 0; i < 12; ++i){
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
        SYSTEMTIME system_creation_time;

        FileTimeToSystemTime(&file_creation_time, &system_creation_time);
        unsigned short year = system_creation_time.wYear;
        unsigned short month = system_creation_time.wMonth;
        unsigned short day = system_creation_time.wDay;
        unsigned short hour = system_creation_time.wHour;
        unsigned short minute = system_creation_time.wMinute;
        unsigned short second = system_creation_time.wSecond;
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
    for(int i = 0; i < name.size(); ++i){
        full_name_archive << Encode(name[i]);
    }
    for(char i : extension){
        full_name_archive << Encode(i);
    }
    for(unsigned short i: date_of_creation){
        for(int j = 8; j >= 0; j -= 8) {
            full_name_archive << Encode(i >> j & 0xFF);
        }
    }
    for(unsigned short i: date_of_last_change){
        for(int j = 8; j >= 0; j -= 8) {
            full_name_archive << Encode(i >> j & 0xFF);
        }
    }
    for(int i = 24; i >= 0; i -= 8){
        full_name_archive << Encode(size >> i & 0xFF);
    }
    while(file){
        char symbol = file.get();
        if(symbol == EOF){
            break;
        }
        full_name_archive << Encode(symbol).to_string();
    }
    full_name_archive.close();
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
    long long size;
    while(full_name_archive) {
        int position = 0;
        for(int i = 0; i < 200; ++i) {
            char buffer[12];
            full_name_archive.read(buffer, 12);
            char decoded_symbol = static_cast<char>(Decode(ToBitset(buffer)));
            if(decoded_symbol == '\a'){
                break;
            }
            probable_name += decoded_symbol;
            ++position;
        }
        full_name_archive.seekg((200-position-1)*12, std::ios::cur);
        if(probable_name == name){
            position = 0;
            for(int i = 0; i < 20; ++i){
                char buffer[12];
                full_name_archive.read(buffer, 12);
                char decoded_symbol = static_cast<char>(Decode(ToBitset(buffer)));
                if(decoded_symbol == '\a'){
                    break;
                }
                extension += decoded_symbol;
                ++position;
            }
            full_name_archive.seekg((20-position-1)*12, std::ios::cur);
            for(int i = 0; i < 6; ++i){
                int value;
                for(int j = 0; j < 2; ++j) {
                    char buffer[12];
                    full_name_archive.read(buffer, 12);
                    if(j == 0) {
                        value = Decode(ToBitset(buffer));
                        value <<= 8;
                    } else {
                        value += Decode(ToBitset(buffer));
                    }
                }
                date_of_creation[i] = value;
            }

            for(int i = 0; i < 6; ++i){
                int value;
                for(int j = 0; j < 2; ++j) {
                    char buffer[12];
                    full_name_archive.read(buffer, 12);
                    if(j == 0) {
                        value = Decode(ToBitset(buffer));
                        value <<= 8;
                    } else {
                        value += Decode(ToBitset(buffer));
                    }
                }
                date_of_last_change[i] = value;
            }
            int value = 0;
            for(int i = 0; i < 4; ++i){
                char buffer[12];
                full_name_archive.read(buffer, 12);
                value <<= 8;
                value |= Decode(ToBitset(buffer));
            }
            size = value; // надо заменить на то как размер в виндоусе хранится
            std::ofstream new_file(file_name);
            if(!new_file.is_open()){
                std::cerr << "There is a problem with creating file";
                exit(EXIT_FAILURE);
            }
            for(long long i = 0; i < size; ++i){
                char buffer[12];
                full_name_archive.read(buffer, 12);
                new_file << static_cast<char>(Decode(ToBitset(buffer)));
            }
            new_file.close();
            SetDateOfCreation(date_of_creation, file_name);
            SetDateOfLastChange(date_of_last_change, file_name);
            break;
        } else {
            full_name_archive.seekg((20+12+12)*12, std::ios::cur);
            int value = 0;
            for(int i = 0; i < 4; ++i){
                char buffer[12];
                full_name_archive.read(buffer, 12);
                value <<= 8;
                value |= Decode(ToBitset(buffer));
            }
            size = value;
            full_name_archive.seekg(size*12, std::ios::cur);
            probable_name.clear();
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
        main_archive.write(buffer, 4096);
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
    while(full_name_archive){
        int position = 0;
        for(int i = 0; i < 200; ++i) {
            char buffer[12];
            full_name_archive.read(buffer, 12);
            char decoded_symbol = static_cast<char>(Decode(ToBitset(buffer)));
            if(decoded_symbol == '\a'){
                break;
            }
            probable_name += decoded_symbol;
            ++position;
        }
        if(probable_name == name){
            long long pos_low_p = full_name_archive.tellp() & MAXLONGLONG - (position+1)*12;
            long long pos_high_p = (full_name_archive.tellp() >> 32) & MAXLONGLONG;
            full_name_archive.seekg((200-position-1)*12, std::ios::cur); // мб объяединить со след строчкой
            full_name_archive.seekg((20+12+12)*12, std::ios::cur);
            int value = 0;
            for(int i = 0; i < 4; ++i){
                char buffer[12];
                full_name_archive.read(buffer, 12);
                value <<= 8;
                value |= Decode(ToBitset(buffer));
            }
            size = value;
            full_name_archive.seekg(size*12, std::ios::cur);
            int cnt = 0;
            bool ok = false;
            long long pos_low_g = full_name_archive.tellg() & MAXLONGLONG;
            long long pos_high_g = full_name_archive.tellg() >> 32 & MAXLONGLONG;
            while(full_name_archive){
                char buffer[13];
                full_name_archive.read(buffer, 12);
                buffer[12] = '\0';
                if(full_name_archive.tellg() != -1) {
                    pos_low_g = full_name_archive.tellg() & MAXLONGLONG;
                    pos_high_g = full_name_archive.tellg() >> 32 & MAXLONGLONG;
                }
                full_name_archive.seekp(pos_high_p << 32 | pos_low_p);
                full_name_archive.write(buffer, 12);
                if(full_name_archive.tellp() != -1) {
                    pos_low_p = full_name_archive.tellp() & MAXLONGLONG;
                    pos_high_p = full_name_archive.tellp() >> 32 & MAXLONGLONG;
                }
                full_name_archive.seekg(pos_high_g << 32 | pos_low_g);
                if(cnt < size) {
                    cnt += strlen(buffer);
                } else if (!ok && cnt >= size*12){
                    ok = true;
                }
            }
            if(!ok){
                std::filesystem::resize_file(archive_name, pos_high_p << 32 | pos_low_p);
                full_name_archive.close();
                break;
            }
        } else {
            full_name_archive.seekg((20+12+12+200-position-1)*12, std::ios::cur);
            int value = 0;
            for(int i = 0; i < 4; ++i){
                char buffer[12];
                full_name_archive.read(buffer, 12);
                value <<= 8;
                value |= Decode(ToBitset(buffer));
            }
            size = value;
            full_name_archive.seekg(size*12, std::ios::cur);
            probable_name.clear();
        }
    }
    full_name_archive.close();
}

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
