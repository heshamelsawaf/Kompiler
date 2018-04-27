cmake_minimum_required(VERSION 3.10)
project(Kompiler)

set(CMAKE_CXX_STANDARD 11)

FIND_PACKAGE(Boost REQUIRED COMPONENTS program_options system filesystem regex)
FIND_PACKAGE(GTest)
find_package(Protobuf REQUIRED)

IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif ()

IF (GTEST_LIBRARY)
    include_directories(${GTEST_INCLUDE_DIR})
else ()
    message("GTest was not found!")
endif ()

IF (Protobuf_FOUND)
    include_directories(${Protobuf_INCLUDE_DIRS})
endif ()


set(Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
set(CFG cfg.cpp cfg.h)
set(CFG_PARSER cfgparser.cpp cfgparser.h)
set(PARSER_MAIN parser_main.cpp ll1_parser.h machine.h)
set(MACHINE machine.cpp machine.h)
set(LEXER lexer.cpp lexer.h machine.h rexparser.h dfa.h trantable.h)
set(ERROR error.cpp error.h)
set(PARSE_TABLE parsetable.cpp parsetable.h)
set(PARSE_TABLE_PB parsetable.pb.cc parsetable.pb.h)
set(LL1_PARSER ll1_parser.cpp ll1_parser.h parsetable.h lexer.h)
set(SENTENTIAL_EXPRESSION sentential_expression.cpp sentential_expression.h)
set(LEFTMOST_DERIVATION leftmost_derivation.cpp leftmost_derivation.h error.h sentential_expression.h)
set(COMMAND_OPTIONS options/CustomOptionDescription.cpp options/CustomOptionDescription.hpp options/OptionPrinter.hpp
        options/OptionPrinter.cpp)

add_executable(Kompiler dfa.cpp dfa.h machine.cpp machine.h main.cpp rexplib.h rexplib.cpp lexer.cpp lexer.h
        trantable.h rexparser.h rexparser.cpp)

add_executable(Parser ${CFG} parser_main.cpp ${CFG_PARSER} ${MACHINE} ${LEXER} ${ERROR} parsetable.pb.cc parsetable.pb.h ${PARSE_TABLE} ${LL1_PARSER}
        ${SENTENTIAL_EXPRESSION} ${LEFTMOST_DERIVATION} ${COMMAND_OPTIONS})


target_link_libraries(Parser ${Boost_LIBRARIES})
target_link_libraries(Parser ${Protobuf_LIBRARIES})

