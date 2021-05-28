#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main(int argc, char** argv) {
    // Validate arguments and get the file into a string
    std::string input;
    if (argc < 2) {
        std::cout << "Bad args" << std::endl;
        return 1;
    } else {
        std::ifstream inputStream(argv[1], std::ifstream::in);
        if (!inputStream.is_open()) {
            std::cout << "Couldn't open the file" << std::endl;
            return 1;
        } else {
            std::ostringstream ss;
            ss << inputStream.rdbuf();
            input = ss.str();
        }
    }

    Lexer* lexer = new Lexer();

    lexer->Run(input);

    DatalogProgram* program = new DatalogProgram();
    Parser* parser = new Parser(lexer->GetTokenVector(), program);
    parser->Run();

    Database* database = new Database();
    Interpreter* interpreter = new Interpreter(program, database);
    interpreter->Run();
    interpreter->Test();
    delete parser;
    delete lexer;
    delete interpreter;//
    delete program;
    delete database;

    return 0;
}