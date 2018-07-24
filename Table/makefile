CC = g++

CFLAGS = -O2

.SUFFIXES: .cpp .o

.cpp.o:
	$(CC) $(CFLAGS) -c $*.cpp

Table_builder: *.o
	$(CC) $(CFLAGS) -o $@ $^

.PHONEY: clean

clean:
	rm Table_builder *.o
