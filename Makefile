GOOGLE_TEST_LIB = gtest
GOOGLE_TEST_INCLUDE = /usr/local/include
OBJS = dfa.o machine.o rexplib.o rexparser.o lexer.o parser.o cfg.o main.o parsetable.o test/test_first.o test/test_ll1.o
LEXOBJS = machine.o lexer.o parser.o lex.o
LEXGENOBJS = machine.o dfa.o rexplib.o rexparser.o lexgen.o
PARSEROBJS = cfg.o parsergen.o parsetable.o
TESTOBJS = cfg.o test/test_first.o test/test_ll1.o test/test_main.o
CC = g++
CFLAGS  = -O2 --std=c++11 -Wall -I $(GOOGLE_TEST_LIB)
DFLAGS = -ggdb
LDFLAGS = -L /usr/local/lib -l $(GOOGLE_TEST_LIB) -l pthread
TARGET = Kompiler
LEXANALYZER = lex
LEXGEN = lexgen
PARSERGEN = parsergen 
TEST = test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
	echo Target $(TARGET) compiled successfully

$(LEXANALYZER): $(LEXOBJS)
	$(CC) $(CFLAGS) $(LEXOBJS) -o $(LEXANALYZER)
	echo Target $(LEXANALYZER) compiled successfully

$(LEXGEN): $(LEXGENOBJS)
	$(CC) $(CFLAGS) $(LEXGENOBJS) -o $(LEXGEN)
	echo Target $(LEXGEN) compiled successfully

$(PARSERGEN): $(PARSEROBJS)
	$(CC) $(CFLAGS) $(DFLAGS) $(PARSEROBJS) -o $(PARSERGEN)
	echo Target $(PARSERGEN) compiled successfully

$(TEST): $(TESTOBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(DFLAGS) $(TESTOBJS) -o test_main
	echo Target $(TEST) compiled successfully

debug: $(OBJS)
	$(CC) $(CFLAGS) $(DFLAGS) $(OBJS) -o $(TARGET)
	echo Target $(TARGET) compiled successfully)

lex.o: lex.cpp machine.h lexer.h parser.h trantable.h
	$(CC) $(CFLAGS) -c lex.cpp

lexgen.o: lexgen.cpp machine.h dfa.h rexparser.h 
	$(CC) $(CFLAGS) -c lexgen.cpp
 
main.o: main.cpp machine.h trantable.h lexer.h parser.h rexparser.h dfa.h
	$(CC) $(CFLAGS) -c main.cpp

dfa.o: dfa.cpp dfa.h machine.h
	$(CC) $(CFLAGS) -c dfa.cpp

machine.o: machine.cpp machine.h
	$(CC) $(CFLAGS) -c machine.cpp

rexparser.o: rexparser.cpp rexparser.h rexplib.h machine.h
	$(CC) $(CFLAGS) -c rexparser.cpp

rexplib.o: rexplib.cpp rexplib.h machine.h
	$(CC) $(CFLAGS) -c rexplib.cpp

lexer.o: lexer.cpp lexer.h machine.h rexparser.h dfa.h trantable.h
	$(CC) $(CFLAGS) -c lexer.cpp

parser.o: parser.cpp parser.h lexer.h
	$(CC) $(CFLAGS) -c parser.cpp

cfg.o: cfg.cpp cfg.h
	$(CC) $(CFLAGS) -c cfg.cpp

parsetable.o: parsetable.cpp parsetable.h
	$(CC) $(CFLAGS) -c parsetable.cpp

parsergen.o: parsergen.cpp cfg.h
	$(CC) $(CFLAGS) -c parsergen.cpp

test/test_main.o: test/test_main.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c test/test_main.cpp
	mv test_main.o test

test/test_first.o: test/test_first.cpp cfg.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c test/test_first.cpp
	mv test_first.o test

test/test_ll1.o: test/test_ll1.cpp cfg.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c test/test_ll1.cpp
	mv test_ll1.o test

clean:
	rm $(OBJS) $(TARGET) *.o $(LEXANALYZER) $(LEXGEN) $(PARSERGEN)