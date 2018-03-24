OBJS = dfa.o machine.o rexplib.o rexparser.o lexer.o parser.o main.o 
LEXOBJS = machine.o lexer.o parser.o lex.o
LEXGENOBJS = machine.o dfa.o rexplib.o rexparser.o lexgen.o
CC = g++
CFLAGS  = -O2 --std=c++11 -Wall
DFLAGS = -ggdb
TARGET = Kompiler
LEXANALYZER = lex
LEXGEN = lexgen


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

clean:
	rm $(OBJS) $(TARGET) lex.o lexgen.o $(LEXANALYZER) $(LEXGEN)
