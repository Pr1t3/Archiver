#include <lib/Archiver.h>
#include <lib/ArgParser.h>
int main(int argc, char** argv) {
    ArgumentParser::ArgParser parser("parser");
    parser.AddFlag('c', "create").Default(false);
    parser.AddStringArgument('f', "file");
    parser.AddFlag('l', "list").Default(false);
    parser.AddFlag('x', "extract").Default(false);
    parser.AddFlag('a', "append").Default(false);
    parser.AddFlag('d', "delete").Default(false);
    parser.AddFlag('A', "concatenate").Default(false);
    parser.AddStringArgument("files").MultiValue();
    parser.AddIntArgument('b', "bits").Default(8);
    if(!parser.Parse(argc, argv)){
        std::cerr << "Something went wrong with parsing";
        exit(EXIT_FAILURE);
    }
    if(parser.GetFlag('c')){
        Archiver::Create(parser.GetStringValue("file"), parser.GetIntValue("bits"));
    }
    if(parser.GetFlag('a')) {
        for (std::string& i: parser.GetStringValues("files")) {
            Archiver::Append(parser.GetStringValue("file"), i);
        }
    }
    if(parser.GetFlag('A')) {
        for (std::string& i: parser.GetStringValues("files")) {
            Archiver::Concatenate(parser.GetStringValue("file"), i);
        }
    }
    if(parser.GetFlag('x')) {
        for (std::string& i: parser.GetStringValues("files")) {
            Archiver::Extract(parser.GetStringValue("file"), i);
        }
    }
    if(parser.GetFlag('d')) {
        for (std::string& i: parser.GetStringValues("files")) {
            Archiver::Delete(parser.GetStringValue("file"), i);
        }
    }
    if(parser.GetFlag('l')) {
        Archiver::List(parser.GetStringValue("file"));
    }
}