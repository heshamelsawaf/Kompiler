GOOGLE_TEST_LIB = gtest
GOOGLE_TEST_INCLUDE = /usr/local/include
OBJS = dfa.o machine.o rexplib.o rexparser.o lexer.o parser.o cfg.o main.o 
LEXOBJS = machine.o lexer.o parser.o lex.o
LEXGENOBJS = machine.o dfa.o rexplib.o rexparser.o lexgen.o
PARSEOBJS = cfg.o parsergen.o
CC = g++
CFLAGS  = -O2 --std=c++11 -Wall -I $(GOOGLE_TEST_LIB)
DFLAGS = -ggdb
TARGET = Kompiler
LEXANALYZER = lex
LEXGEN = lexgen
PARSERGEN = parsergen 

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

$(PARSEGEN): $(PARSEOBJS)
	$(CC) $(CFLAGS) $(PARSEOBJS) -o $(PARSEGEN)
	echo Target $(PARSEGEN) compiled successfully

debug: $(OBJS)
	$(CC) $(CFLAGS) $(DFLAGS) $(OBJS) -o $(TARGET)
	echo Target $(TARGET) compiled successfully

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

parsergen.o: parsergen.cpp
	$(CC) $(CFLAGS) -c parsergen.cpp

clean:
	rm $(OBJS) $(TARGET) lex.o lexgen.o parsergen.o $(LEXANALYZER) $(LEXGEN) $(PARSEGEN)
