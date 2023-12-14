#include "../lib/Archiver.h"
#include <Windows.h>
#include <filesystem>
int main() {
    /*const char* fileNam = "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\1.txt";
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring f = converter.from_bytes(fileNam);

    const wchar_t* fileName = f.c_str();
    // Используйте wideString как const wchar_t*

    // Освобождение выделенной памяти

    FILETIME newCreateTime;

    // Получаем текущую системную дату и время
    SYSTEMTIME st;
    st.wYear = 2010;
    st.wDay = 10;
    st.wDayOfWeek = 1;
    st.wMinute = 56;
    st.wMonth = 3;
    st.wHour = 16;
    st.wSecond = 13;

    // Задаем новую дату создания файла
    SystemTimeToFileTime(&st, &newCreateTime);

    // Открываем файл с правами доступа FILE_WRITE_ATTRIBUTES
    HANDLE hFile = CreateFileW(fileName, FILE_WRITE_ATTRIBUTES, FILE_SHARE_WRITE | FILE_SHARE_READ,
                               nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Ошибка открытия файла: " << GetLastError() << std::endl;
        return 1;
    }
    auto a = GetFileTime(hFile, nullptr, nullptr, nullptr);
    // Устанавливаем новую дату создания файла
    if (!SetFileTime(hFile, &newCreateTime, &newCreateTime, &newCreateTime)) {
        std::cerr << "Ошибка установки новой даты создания файла: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return 1;
    }
    // Закрываем файл
    CloseHandle(hFile);
    delete[] fileName;*/
    /*const char* fileNam = "C:\\Users\\romap\\Downloads\\Desktop.rar";
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring f = converter.from_bytes(fileNam);
    const wchar_t* fileName = f.c_str();
    WIN32_FILE_ATTRIBUTE_DATA fileAttributes;
    if (GetFileAttributesExW(fileName, GetFileExInfoStandard, &fileAttributes)) {
        FILETIME ftCreationTime = fileAttributes.ftCreationTime;
        FILETIME ftLastAccessTime = fileAttributes.ftLastAccessTime;
        FILETIME ftLastWriteTime = fileAttributes.ftLastWriteTime;
        std::cout << fileAttributes.nFileSizeLow << '\n';
        std::cout << fileAttributes.nFileSizeHigh << '\n';
        SYSTEMTIME stCreationTime, stLastAccessTime, stLastWriteTime;

        FileTimeToSystemTime(&ftCreationTime, &stCreationTime);
        FileTimeToSystemTime(&ftLastAccessTime, &stLastAccessTime);
        FileTimeToSystemTime(&ftLastWriteTime, &stLastWriteTime);

        std::cout << "Время создания: "
                   << stCreationTime.wYear << "-" << stCreationTime.wMonth << "-" << stCreationTime.wDay
                   << " " << stCreationTime.wHour << ":" << stCreationTime.wMinute << ":" << stCreationTime.wSecond << std::endl;

        std::wcout << L"Время последнего доступа: "
                   << stLastAccessTime.wYear << L"-" << stLastAccessTime.wMonth << L"-" << stLastAccessTime.wDay
                   << L" " << stLastAccessTime.wHour << L":" << stLastAccessTime.wMinute << L":" << stLastAccessTime.wSecond << std::endl;

        std::wcout << L"Время последней модификации: "
                   << stLastWriteTime.wYear << L"-" << stLastWriteTime.wMonth << L"-" << stLastWriteTime.wDay
                   << L" " << stLastWriteTime.wHour << L":" << stLastWriteTime.wMinute << L":" << stLastWriteTime.wSecond << std::endl;
    } else {
        std::cerr << "Ошибка получения атрибутов файла: " << GetLastError() << std::endl;
        return 1;
    }*/
    Archiver arch;
    /*
    arch.Create("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first");
    arch.Create("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\second");
    arch.Append("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\1.txt");
    arch.Append("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\second.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\2.txt");
    */
    //arch.Concatenate("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\second.haf");
    //arch.Extract("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\2.txt");
    //arch.Extract("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\new_archive.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\3.txt");
    //arch.Create("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\new_archive");
    //arch.Append("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\new_archive.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\Лекция 2.txt");
    //arch.Append("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\new_archive.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\2.txt");

    //arch.Append("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\2.txt");
    //arch.Delete("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\Лекция 2.txt");
    //arch.Delete("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\1.txt");
    //arch.Append("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\2.txt");
    //arch.Append("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\1.txt");
    //arch.Append("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\1.txt");
    /*
    arch.Append("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\1.txt");
    arch.Append("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\2.txt");
    arch.Append("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\Лекция 2.txt");
    arch.List("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf");
    */

    arch.Delete("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\Лекция 2.txt");
    arch.Extract("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\3.txt");
    arch.Extract("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\Лекция 2.txt");

    //arch.Delete("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\Лекция 2.txt");
    //arch.Extract("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\Лекция 2.txt");
    //arch.Concatenate("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\second.haf");

    //arch.Append("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\3.txt");
    //std::cout << (int)'\a';

    //arch.Delete("C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\first.haf", "C:\\Users\\romap\\Downloads\\MainFolder\\Программы\\Git\\labwork6-Pr1t3\\archives\\1.txt");
}
// 3264