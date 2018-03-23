OBJS = dfa.o machine.o rexplib.o rexparser.o main.o
CC = g++
CFLAGS  = -O2 --std=c++11 -Wall
DFLAGS = -ggdb
TARGET = Kompiler

all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
	echo Target $(TARGET) compiled successfully

debug: $(OBJS)
	$(CC) $(CFLAGS) $(DFLAGS) $(OBJS) -o $(TARGET)
	echo Target $(TARGET) compiled successfully

main.o: main.cpp machine.h rexplib.h rexparser.h dfa.h
	$(CC) $(CFLAGS) -c main.cpp

dfa.o: dfa.cpp dfa.h machine.h
	$(CC) $(CFLAGS) -c dfa.cpp

machine.o: machine.cpp machine.h
	$(CC) $(CFLAGS) -c machine.cpp

rexparser.o: rexparser.cpp rexparser.h rexplib.h machine.h
	$(CC) $(CFLAGS) -c rexparser.cpp

rexplib.o: rexplib.cpp rexplib.h machine.h
	$(CC) $(CFLAGS) -c rexplib.cpp

clean:
	rm $(OBJS) $(TARGET)
