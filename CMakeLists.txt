cmake_minimum_required(VERSION 3.16.3)
project(Project3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Project3
        Automaton.h
        CommentAutomaton.cpp
        CommentAutomaton.h
        Database.cpp
        Database.h
        DatalogProgram.cpp
        DatalogProgram.h
        Header.cpp
        Header.h
        IDAutomaton.cpp
        IDAutomaton.h
        Interpreter.cpp
        Interpreter.h
        Lexer.cpp
        Lexer.h
        main.cpp
        MatcherAutomaton.cpp
        MatcherAutomaton.h
        Parameter.cpp
        Parameter.h
        Parser.cpp
        Parser.h
        Predicate.cpp
        Predicate.h
        Relation.cpp
        Relation.h
        Rule.cpp
        Rule.h
        StringAutomaton.cpp
        StringAutomaton.h
        Token.cpp
        Token.h
        Tuple.cpp
        Tuple.h)
