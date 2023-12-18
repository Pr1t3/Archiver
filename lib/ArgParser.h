#pragma once

#include <string>
#include <vector>
#include <iostream>

namespace ArgumentParser {

    template<typename T>
    class Parameter {
    private:
        int min_count{};
        std::string name;
        char short_name{};
        std::string description;
        std::vector<T> values;
        std::vector<T>* outside_array_pointer;
        T* outside_value_pointer;
        T default_value;
        bool is_array_outside{};
        bool is_value_outside{};
        bool is_default{};
        bool is_repeated{};
    public:
        explicit Parameter() {};

        explicit Parameter(std::string name, std::string description = "") : name(std::move(name)), min_count(0),
                                                                             description(std::move(description)) {}

        explicit Parameter(char short_name, std::string name, std::string description = "") : name(std::move(name)),
                                                                                              short_name(short_name),
                                                                                              min_count(0), description(std::move(description)) {}

        void SetName(const std::string& cur_name) {
            name = cur_name;
        }

        void SetShortName(char symbol) {
            short_name = symbol;
        }

        void CopyElementsToArray() {
            if (is_array_outside && values.size() > 0) {
                outside_array_pointer->insert(outside_array_pointer->begin(), values.begin(), values.end());
            }
        }

        void CopyElementToValue() {
            if (is_value_outside && values.size() > 0) {
                *(outside_value_pointer) = values[0];
            }
        }

        std::vector<T>& GetValues() {
            return values;
        }

        void AddValue(T value) {
            if (!is_default) {
                values.push_back(value);
            } else {
                values[0] = value;
            }
        }

        void SetArrayPointer(std::vector<T>& array) {
            outside_array_pointer = &array;
            is_array_outside = true;
        }

        void SetValuePointer(T& value) {
            outside_value_pointer = &value;
            is_value_outside = true;
        }

        std::string GetName() {
            return name;
        }

        char GetShortName() {
            return short_name;
        }

        void SetMinCount(int n) {
            min_count = n;
        }

        int GetMinCount() {
            return min_count;
        }

        void SetIsDefault() {
            is_default = true;
        }

        bool GetIsDefault() {
            return is_default;
        }

        void AddDescription(const std::string& desc) {
            description = desc;
        }

        std::string GetDescription() {
            return description;
        }

        bool GetIsRepeated() {
            return is_repeated;
        }

        void SetIsRepeated() {
            is_repeated = true;
        }

        T GetDefaultValue() {
            return default_value;
        }

        void SetDefaultValue(T value) {
            default_value = value;
        }
    };

    enum TypesOfData {
        string_type,
        int_type,
        bool_type
    };

    class ArgParser {
    private:
        std::vector<Parameter<std::string>> string_arguments;
        std::vector<Parameter<int>> int_arguments;
        std::vector<Parameter<bool>> bool_arguments;
        Parameter<std::string> help;
        const std::string name_of_parser;
        int last_used{};
        bool is_positional{};
    public:
        explicit ArgParser(std::string name_of_parser);

        bool Parse(const std::vector<std::string>& arguments);

        bool Parse(int argc, char** argv);

        std::string GetNameOfTheParser();

        ArgParser& AddStringArgument(char* name, const char* description = "");

        ArgParser& AddStringArgument(char short_name, char* name, const char* description = "");

        ArgParser& AddIntArgument(char* name, const char* description = "");

        ArgParser& AddIntArgument(char short_name, char* name, const char* description = "");

        ArgParser& AddFlag(char* name, const char* description = "");

        ArgParser& AddFlag(char short_name, char* name, const char* description = "");

        ArgParser& AddHelp(char* name, char* description);

        ArgParser& AddHelp(char short_name, char* name, char* description);

        void HelpDescription();

        std::string GetStringValue(const std::string& name, int index = 0);

        std::string GetStringValue(char name, int index = 0);

        std::vector<std::string> GetStringValues(const std::string& name);

        std::vector<std::string> GetStringValues(char name);

        int GetIntValue(const std::string& name, int index = 0);

        int GetIntValue(char name, int index = 0);

        bool GetFlag(const std::string& name, int index = 0);

        bool GetFlag(char name, int index = 0);

        ArgParser& AddStringValue(const std::string& name, const std::string& value);

        ArgParser& AddStringValue(char name, const std::string& value);

        ArgParser& AddIntValue(const std::string& name, int value);

        ArgParser& AddIntValue(char name, int value);

        ArgParser& AddBoolValue(const std::string& name, bool value);

        ArgParser& AddBoolValue(char name, bool value);

        ArgParser& Default(char* value);

        ArgParser& Default(int value);

        ArgParser& Default(bool value);

        ArgParser& StoreValues(std::vector<std::string>& array);

        ArgParser& StoreValues(std::vector<int>& array);

        ArgParser& StoreValues(std::vector<bool>& array);

        ArgParser& StoreValue(std::string& string);

        ArgParser& StoreValue(int& num);

        ArgParser& StoreValue(bool& flag);

        ArgParser& MultiValue();

        ArgParser& MultiValue(int minCount);

        ArgParser& Positional();

        bool Help();

        void PlaceValue(std::string name, std::string value);

        void PlaceValue(char name, std::string value);

        void PlaceValue(const std::string& name);

        void PlaceValue(char name);

        bool IsThereSuchArgument(char arg);

        bool IsThereSuchArgument(const std::string& arg);

        bool Check();

        void CopyElementsToOutside();
    };

}