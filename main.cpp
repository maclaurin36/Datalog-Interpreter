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
    delete parser;
    delete lexer;
    delete interpreter;

    /*Relation* newRelation = new Relation();
    Tuple tuple1;
    Tuple tuple2;
    Tuple tuple3;
    Header* header = new Header();
    header->AddAttribute("Attribute1");
    header->AddAttribute("Attribute2");
    header->AddAttribute("Attribute3");
    tuple1.AddElement("b");
    tuple1.AddElement("a");
    tuple1.AddElement("a");
    tuple2.AddElement("c");
    tuple2.AddElement("d");
    tuple2.AddElement("d");
    tuple3.AddElement("g");
    tuple3.AddElement("h");
    tuple3.AddElement("i");
    newRelation->SetName("RelationName");
    newRelation->SetHeader(header);
    newRelation->AddTuple(tuple1);
    newRelation->AddTuple(tuple2);
    newRelation->AddTuple(tuple3);
    std::cout << newRelation->toString() << std::endl;
    std::vector<std::string> someVector;
    someVector.push_back("New name 1");
    someVector.push_back("New name 2");
    someVector.push_back("New name 3");
    Relation* newRelation2 = newRelation->Rename(&someVector);
    newRelation2->SetName("otherName");
    std::cout << newRelation2->toString() << std::endl;
    Database* newDatabase = new Database;
    std::cout << std::endl;
    std::cout << newDatabase->toString();*/
    return 0;
}