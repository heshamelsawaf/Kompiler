GOOGLE_TEST_LIB = gtest
OBJS = dfa.o machine.o rexplib.o rexparser.o lexer.o tokenizer.o cfg.o main.o parsetable.o test/test_all.o test/test_ll1.o parsetable.pb.o
LEXOBJS = machine.o lexer.o tokenizer.o lex.o
LEXGENOBJS = machine.o dfa.o rexplib.o rexparser.o lexgen.o
PARSERGENOBJS = cfg.o parsergen.o cfgparser.o machine.o lexer.o error.o parsetable.o parsetable.pb.o ll1_parser.o sentential_expression.o leftmost_derivation.o
PARSEROBJS = cfg.o parser_main.o machine.o lexer.o error.o parsetable.o parsetable.pb.o ll1_parser.o cfgparser.o sentential_expression.o leftmost_derivation.o
TESTOBJS = cfg.o ll1_parser.o machine.o lexer.o parsetable.o error.o parsetable.pb.o sentential_expression.o leftmost_derivation.o cfgparser.o test/test_all.o test/test_ll1.o test/test_main.o
CC = g++
CFLAGS  = -O2 --std=c++11 -Wall
DFLAGS = -ggdb
LDFLAGS = -l $(GOOGLE_TEST_LIB) -l pthread
PBFLAGS = -lprotoc -lprotobuf -lpthread
TARGET = Kompiler
LEXANALYZER = lex
LEXGEN = lexgen
PARSERGEN = parsergen 
PARSER = parser
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

$(PARSERGEN): $(PARSERGENOBJS)
	$(CC) $(CFLAGS) $(DFLAGS) $(PARSERGENOBJS) -o $(PARSERGEN) $(PBFLAGS)
	echo Target $(PARSERGEN) compiled successfully

$(PARSER): $(PARSEROBJS)
	$(CC) $(CFLAGS) $(DFLAGS) $(PARSEROBJS) -o $(PARSER) $(PBFLAGS)
	echo Target $(PARSER) compiled successfully


$(TEST): $(TESTOBJS)
	$(CC) $(CFLAGS) $(DFLAGS) $(TESTOBJS) -o test_main $(LDFLAGS) $(PBFLAGS)
	echo Target $(TEST) compiled successfully

debug: $(OBJS)
	$(CC) $(CFLAGS) $(DFLAGS) $(OBJS) -o $(TARGET)
	echo Target $(TARGET) compiled successfully)

lex.o: lex.cpp machine.h lexer.h tokenizer.h trantable.h
	$(CC) $(CFLAGS) -c lex.cpp

lexgen.o: lexgen.cpp machine.h dfa.h rexparser.h 
	$(CC) $(CFLAGS) -c lexgen.cpp
 
main.o: main.cpp machine.h trantable.h lexer.h tokenizer.h rexparser.h dfa.h
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

tokenizer.o: tokenizer.cpp tokenizer.h lexer.h
	$(CC) $(CFLAGS) -c tokenizer.cpp

cfg.o: cfg.cpp cfg.h
	$(CC) $(DFLAGS) $(CFLAGS) -c cfg.cpp

cfgparser.o: cfg.h cfgparser.h cfgparser.cpp
	$(CC) $(DFLAGS) $(CFLAGS) -c cfgparser.cpp

parsetable.o: parsetable.cpp parsetable.h
	$(CC) $(CFLAGS) -c parsetable.cpp

parsetable.pb.o: parsetable.pb.cc parsetable.pb.h
	$(CC) $(CFLAGS) -c parsetable.pb.cc

parsergen.o: parsergen.cpp cfg.h
	$(CC) $(DFLAGS) $(CFLAGS) -c parsergen.cpp

ll1_parser.o: ll1_parser.cpp ll1_parser.h parsetable.h lexer.h
	$(CC) $(DFLAGS) $(CFLAGS) -c ll1_parser.cpp

error.o: error.cpp error.h
	$(CC) $(DFLAGS) $(CFLAGS) -c error.cpp

sentential_expression.o: sentential_expression.cpp sentential_expression.h
	$(CC) $(DFLAGS) $(CFLAGS) -c sentential_expression.cpp

leftmost_derivation.o: leftmost_derivation.cpp leftmost_derivation.h error.h sentential_expression.h
	$(CC) $(DFLAGS) $(CFLAGS) -c leftmost_derivation.cpp

parser_main.o: parser_main.cpp ll1_parser.h machine.h 
	$(CC) $(DFLAGS) $(CFLAGS) -c parser_main.cpp

test/test_main.o: test/test_main.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c test/test_main.cpp
	mv test_main.o test

test/test_all.o: test/test_all.cpp cfg.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c test/test_all.cpp
	mv test_all.o test

test/test_ll1.o: test/test_ll1.cpp cfg.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c test/test_ll1.cpp
	mv test_ll1.o test

clean:
	rm -r $(OBJS) $(TARGET) *.o $(LEXANALYZER) $(LEXGEN) $(PARSERGEN)