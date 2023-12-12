#include "ArgParser.h"

bool ArgumentParser::ArgParser::Parse(const std::vector<std::string>& arguments) {
    for (int i = 1; i < arguments.size(); ++i) {
        if (is_positional) {
            if (last_used == TypesOfData::int_type) {
                int_arguments[0].AddValue(std::stoi(arguments[i]));
            } else if (last_used == TypesOfData::string_type) {
                string_arguments[0].AddValue(arguments[i]);
            } else if (last_used == TypesOfData::bool_type) {
                bool_arguments[0].AddValue(static_cast<bool>(std::stoi(arguments[i])));
            }
        } else {
            int delimiter_pos = arguments[i].find('=');
            int start_index = 0;
            while (arguments[i][start_index] == '-' && start_index < 2) {
                ++start_index;
            }
            if (delimiter_pos != std::string::npos) {
                std::string name = arguments[i].substr(start_index, delimiter_pos - start_index);
                std::string value = arguments[i].substr(delimiter_pos + 1);
                if (name.size() > 1) {
                    PlaceValue(name, value);
                } else {
                    PlaceValue(name[0], value);
                }
            } else {
                std::string name = arguments[i].substr(start_index);
                if (name.size() > 1) {
                    if (std::all_of(name.begin(), name.end(), [this](char i) { return IsThereSuchArgument(i); })) {
                        std::for_each(name.begin(), name.end(), [this](char i) { PlaceValue(i); });
                    } else {
                        PlaceValue(name);
                    }
                } else {
                    PlaceValue(name[0]);
                }
            }
        }
    }
    CopyElementsToOutside();
    return Check();
}

bool ArgumentParser::ArgParser::Parse(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        std::string argument = std::string(argv[i]);
        if (is_positional) {
            if (last_used == TypesOfData::int_type) {
                int_arguments[0].AddValue(std::stoi(argument));
            } else if (last_used == TypesOfData::string_type) {
                string_arguments[0].AddValue(argument);
            } else if (last_used == TypesOfData::bool_type) {
                bool_arguments[0].AddValue(static_cast<bool>(std::stoi(argument)));
            }
        } else {
            int delimiter_pos = argument.find('=');
            int start_index = 0;
            while (argument[start_index] == '-' && start_index < 2) {
                ++start_index;
            }
            if (delimiter_pos != std::string::npos) {
                std::string name = argument.substr(start_index, delimiter_pos - start_index);
                std::string value = argument.substr(delimiter_pos + 1);
                if (name.size() > 1) {
                    PlaceValue(name, value);
                } else {
                    PlaceValue(name[0], value);
                }
            } else {
                std::string name = argument.substr(start_index);
                if (name.size() > 1) {
                    if (std::all_of(name.begin(), name.end(), [this](char i) { return IsThereSuchArgument(i); })) {
                        std::for_each(name.begin(), name.end(), [this](char i) { PlaceValue(i); });
                    } else {
                        PlaceValue(name);
                    }
                } else {
                    PlaceValue(name[0]);
                }
            }
        }
    }
    CopyElementsToOutside();
    return Check();
}

ArgumentParser::ArgParser::ArgParser(std::string name_of_parser) : name_of_parser(std::move(name_of_parser)) {}

std::string ArgumentParser::ArgParser::GetNameOfTheParser() {
    return name_of_parser;
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::AddIntArgument(char* name, const char* description) {
    int_arguments.emplace_back(std::string(name), std::string(description));
    last_used = TypesOfData::int_type;
    return *this;
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::AddStringArgument(char* name, const char* description) {
    string_arguments.emplace_back(std::string(name), std::string(description));
    last_used = TypesOfData::string_type;
    return *this;
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::AddFlag(char* name, const char* description) {
    bool_arguments.emplace_back(std::string(name), std::string(description));
    last_used = TypesOfData::bool_type;
    return *this;
}

ArgumentParser::ArgParser&
ArgumentParser::ArgParser::AddIntArgument(char short_name, char* name, const char* description) {
    int_arguments.emplace_back(short_name, std::string(name), std::string(description));
    last_used = TypesOfData::int_type;
    return *this;
}

ArgumentParser::ArgParser&
ArgumentParser::ArgParser::AddStringArgument(char short_name, char* name, const char* description) {
    string_arguments.emplace_back(short_name, std::string(name), std::string(description));
    last_used = TypesOfData::string_type;
    return *this;
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::AddFlag(char short_name, char* name, const char* description) {
    bool_arguments.emplace_back(short_name, std::string(name), std::string(description));
    last_used = TypesOfData::bool_type;
    return *this;
}

std::string ArgumentParser::ArgParser::GetStringValue(const std::string& name, int index) {
    for (auto& string_argument : string_arguments) {
        if (string_argument.GetName() == name) {
            if (string_argument.GetValues().size() > index) {
                return string_argument.GetValues()[index];
            }
            std::cerr << "There is no value with such index";
            exit(EXIT_FAILURE);
        }
    }
    std::cerr << "There is no argument with such name";
    exit(EXIT_FAILURE);
}

std::string ArgumentParser::ArgParser::GetStringValue(const char name, int index) {
    for (auto& string_argument : string_arguments) {
        if (string_argument.GetShortName() == name) {
            if (!string_argument.GetValues().empty()) {
                return string_argument.GetValues()[0];
            }
            std::cerr << "There is no value with such index";
            exit(EXIT_FAILURE);
        }
    }
    std::cerr << "There is no argument with such name";
    exit(EXIT_FAILURE);
}

int ArgumentParser::ArgParser::GetIntValue(const std::string& name, int index) {
    for (auto& int_argument : int_arguments) {
        if (!(int_argument.GetName() == name)) {
            continue;
        }
        if (int_argument.GetValues().size() > index) {
            return int_argument.GetValues()[index];
        }
        std::cerr << "There is no value with such index";
        exit(EXIT_FAILURE);
    }
    std::cerr << "There is no argument with such name";
    exit(EXIT_FAILURE);
}

int ArgumentParser::ArgParser::GetIntValue(const char name, int index) {
    for (auto& int_argument : int_arguments) {
        if (int_argument.GetShortName() != name) {
            continue;
        }
        if (!int_argument.GetValues().empty()) {
            return int_argument.GetValues()[0];
        }
        std::cerr << "There is no value with such index";
        exit(EXIT_FAILURE);
    }
    std::cerr << "There is no argument with such name";
    exit(EXIT_FAILURE);
}

bool ArgumentParser::ArgParser::GetFlag(const std::string& name, int index) {
    for (auto& bool_argument : bool_arguments) {
        if (!(bool_argument.GetName() == name)) {
            continue;
        }
        if (bool_argument.GetValues().size() > index) {
            return bool_argument.GetValues()[index];
        }
        std::cerr << "There is no value with such index";
        exit(EXIT_FAILURE);
    }
    std::cerr << "There is no argument with such name";
    exit(EXIT_FAILURE);
}

bool ArgumentParser::ArgParser::GetFlag(const char name, int index) {
    for (auto& bool_argument : bool_arguments) {
        if (bool_argument.GetShortName() != name) {
            continue;
        }
        if (!bool_argument.GetValues().empty()) {
            return bool_argument.GetValues()[0];
        }
        std::cerr << "There is no value with such index";
        exit(EXIT_FAILURE);
    }
    std::cerr << "There is no argument with such name";
    exit(EXIT_FAILURE);
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::AddHelp(char* name, char* description) {
    help.SetName(std::string(name));
    help.AddDescription(std::string(description));
    return *this;
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::AddHelp(char short_name, char* name, char* description) {
    help.SetName(std::string(name));
    help.SetShortName(short_name);
    help.AddDescription(std::string(description));
    return *this;
}

void ArgumentParser::ArgParser::HelpDescription() {
    std::cout << name_of_parser << '\n';
    for (auto& string_argument : string_arguments) {
        std::cout << string_argument.GetShortName() << ' ' << string_argument.GetName() << ' ' << "<std::string>" << ' '
                  << string_argument.GetDescription() << ' ';
        if (string_argument.GetIsRepeated()) {
            std::cout << "[repeated, min args = " << string_argument.GetMinCount() << "] ";
        }
        if (string_argument.GetIsDefault()) {
            std::cout << "[default = " << string_argument.GetDefaultValue() << "] ";
        }
        std::cout << '\n';
    }
    for (auto& int_argument : int_arguments) {
        std::cout << int_argument.GetShortName() << ' ' << int_argument.GetName() << ' ' << "<int>" << ' '
                  << int_argument.GetDescription() << ' ';
        if (int_argument.GetIsRepeated()) {
            std::cout << "[repeated, min args = " << int_argument.GetMinCount() << "] ";
        }
        if (int_argument.GetIsDefault()) {
            std::cout << "[default = " << int_argument.GetDefaultValue() << "] ";
        }
        std::cout << '\n';
    }
    for (auto& bool_argument : bool_arguments) {
        std::cout << bool_argument.GetShortName() << ' ' << bool_argument.GetName() << ' ' << "<bool>" << ' '
                  << bool_argument.GetDescription() << ' ';
        if (bool_argument.GetIsRepeated()) {
            std::cout << "[repeated, min args = " << bool_argument.GetMinCount() << "] ";
        }
        if (bool_argument.GetIsDefault()) {
            std::cout << "[default = " << bool_argument.GetDefaultValue() << "] ";
        }
        std::cout << '\n';
    }
    std::cout << help.GetShortName() << ' ' << help.GetName() << ' ' << help.GetDescription() << '\n';
}

ArgumentParser::ArgParser&
ArgumentParser::ArgParser::AddStringValue(const std::string& name, const std::string& value) {
    for (auto& string_argument : string_arguments) {
        if (string_argument.GetName() == name) {
            string_argument.AddValue(value);
            return *this;
        }
    }
    std::cerr << "There is no argument with such name";
    exit(EXIT_FAILURE);
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::AddStringValue(char name, const std::string& value) {
    for (auto& string_argument : string_arguments) {
        if (string_argument.GetShortName() == name) {
            string_argument.AddValue(value);
            return *this;
        }
    }
    std::cerr << "There is no argument with such name";
    exit(EXIT_FAILURE);
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::AddIntValue(const std::string& name, int value) {
    for (auto& int_argument : int_arguments) {
        if (int_argument.GetName() == name) {
            int_argument.AddValue(value);
            return *this;
        }
    }
    std::cerr << "There is no argument with such name";
    exit(EXIT_FAILURE);
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::AddIntValue(char name, int value) {
    for (auto& int_argument : int_arguments) {
        if (int_argument.GetShortName() == name) {
            int_argument.AddValue(value);
            return *this;
        }
    }
    std::cerr << "There is no argument with such name";
    exit(EXIT_FAILURE);
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::AddBoolValue(const std::string& name, bool value) {
    for (auto& bool_argument : bool_arguments) {
        if (bool_argument.GetName() == name) {
            bool_argument.AddValue(value);
            return *this;
        }
    }
    std::cerr << "There is no argument with such name";
    exit(EXIT_FAILURE);
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::AddBoolValue(char name, bool value) {
    for (auto& bool_argument : bool_arguments) {
        if (bool_argument.GetShortName() == name) {
            bool_argument.AddValue(value);
            return *this;
        }
    }
    std::cerr << "There is no argument with such name";
    exit(EXIT_FAILURE);
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::Default(char* value) {
    string_arguments.back().AddValue(std::string(value));
    string_arguments.back().SetIsDefault();
    string_arguments.back().SetDefaultValue(std::string(value));
    return *this;
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::Default(int value) {
    int_arguments.back().AddValue(value);
    int_arguments.back().SetIsDefault();
    int_arguments.back().SetDefaultValue(value);
    return *this;
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::Default(bool value) {
    bool_arguments.back().AddValue(value);
    bool_arguments.back().SetIsDefault();
    bool_arguments.back().SetDefaultValue(value);
    return *this;
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::StoreValues(std::vector<std::string>& array) {
    string_arguments.back().SetArrayPointer(array);
    return *this;
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::StoreValues(std::vector<int>& array) {
    int_arguments.back().SetArrayPointer(array);
    return *this;
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::StoreValues(std::vector<bool>& array) {
    bool_arguments.back().SetArrayPointer(array);
    return *this;
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::StoreValue(std::string& string) {
    string = "0";
    string_arguments.back().SetValuePointer(string);
    return *this;
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::StoreValue(int& num) {
    num = 0;
    int_arguments.back().SetValuePointer(num);
    return *this;
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::StoreValue(bool& flag) {
    flag = false;
    bool_arguments.back().SetValuePointer(flag);
    return *this;
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::MultiValue() {
    if (last_used == TypesOfData::int_type) {
        int_arguments.back().SetIsRepeated();
    } else if (last_used == TypesOfData::string_type) {
        string_arguments.back().SetIsRepeated();
    } else if (last_used == TypesOfData::bool_type) {
        bool_arguments.back().SetIsRepeated();
    }
    return *this;
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::MultiValue(int minCount) {
    if (last_used == TypesOfData::int_type) {
        int_arguments.back().SetMinCount(minCount);
        int_arguments.back().SetIsRepeated();
    } else if (last_used == TypesOfData::string_type) {
        string_arguments.back().SetMinCount(minCount);
        string_arguments.back().SetIsRepeated();
    } else if (last_used == TypesOfData::bool_type) {
        bool_arguments.back().SetMinCount(minCount);
        bool_arguments.back().SetIsRepeated();
    }
    return *this;
}

ArgumentParser::ArgParser& ArgumentParser::ArgParser::Positional() {
    is_positional = true;
    return *this;
}

bool ArgumentParser::ArgParser::Help() {
    return !(help.GetDescription().empty());
}

void ArgumentParser::ArgParser::PlaceValue(const std::string& name, const std::string& value) {
    for (auto& int_argument : int_arguments) {
        if (int_argument.GetName() == name) {
            int_argument.AddValue(std::stoi(value));
            return;
        }
    }
    for (auto& string_argument : string_arguments) {
        if (string_argument.GetName() == name) {
            string_argument.AddValue(std::string(value));
            return;
        }
    }
    for (auto& bool_argument : bool_arguments) {
        if (bool_argument.GetName() == name) {
            bool_argument.AddValue(static_cast<bool>(std::stoi(value)));
            return;
        }
    }
}

void ArgumentParser::ArgParser::PlaceValue(char name, const std::string& value) {
    for (auto& int_argument : int_arguments) {
        if (int_argument.GetShortName() == name) {
            int_argument.AddValue(std::stoi(value));
            return;
        }
    }
    for (auto& string_argument : string_arguments) {
        if (string_argument.GetShortName() == name) {
            string_argument.AddValue(std::string(value));
            return;
        }
    }
    for (auto& bool_argument : bool_arguments) {
        if (bool_argument.GetShortName() == name) {
            bool_argument.AddValue(static_cast<bool>(std::stoi(value)));
            return;
        }
    }
}

void ArgumentParser::ArgParser::PlaceValue(const std::string& name) {
    for (auto& bool_argument : bool_arguments) {
        if (bool_argument.GetName() == name) {
            bool_argument.AddValue(true);
            return;
        }
    }
}

void ArgumentParser::ArgParser::PlaceValue(const char name) {
    for (auto& bool_argument : bool_arguments) {
        if (bool_argument.GetShortName() == name) {
            bool_argument.AddValue(true);
        }
    }
}

bool ArgumentParser::ArgParser::IsThereSuchArgument(char arg) {
    return std::any_of(bool_arguments.begin(), bool_arguments.end(),
                       [this, arg](Parameter<bool> param) { return param.GetShortName() == arg; });
}

bool ArgumentParser::ArgParser::Check() {
    if (std::any_of(string_arguments.begin(), string_arguments.end(),
                    [this](Parameter<std::string> param) {
                        return ((param.GetValues().size() < param.GetMinCount() ||
                                 param.GetValues().empty()) && param.GetDescription().empty());
                    })) {
        return false;
    }
    if (std::any_of(int_arguments.begin(), int_arguments.end(),
                    [this](Parameter<int> param) {
                        return ((param.GetValues().size() < param.GetMinCount() ||
                                 param.GetValues().empty()) && param.GetDescription().empty());
                    })) {
        return false;
    }
    if (std::any_of(bool_arguments.begin(), bool_arguments.end(),
                    [this](Parameter<bool> param) {
                        return ((param.GetValues().size() < param.GetMinCount() ||
                                 param.GetValues().empty()) && param.GetDescription().empty());
                    })) {
        return false;
    }
    return true;
}

void ArgumentParser::ArgParser::CopyElementsToOutside() {
    std::for_each(string_arguments.begin(), string_arguments.end(), [this](Parameter<std::string> param) {
        param.CopyElementToValue();
        param.CopyElementsToArray();
    });
    std::for_each(int_arguments.begin(), int_arguments.end(), [this](Parameter<int> param) {
        param.CopyElementToValue();
        param.CopyElementsToArray();
    });
    std::for_each(bool_arguments.begin(), bool_arguments.end(), [this](Parameter<bool> param) {
        param.CopyElementToValue();
        param.CopyElementsToArray();
    });
}
