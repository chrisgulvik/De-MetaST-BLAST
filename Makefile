CC = g++
CFLAGS = -g

PROG: De-MetaST-BLAST BLASTxml_to_tsv
all: $(PROG)

.cpp.o:
	$(CC) $(CFLAGS) -c $*.cpp
De-MetaST-BLAST: De-MetaST-BLAST.o PrimerDriver.o PrimerLib.o
	$(CC) $(CFLAGS) -o $@ *.o

BLASTxml_to_tsv:
	cd Table && $(CC) $(CFLAGS) -c BLASTxml_to_tsv.cpp
	cd Table && $(CC) $(CFLAGS) -o $@ BLASTxml_to_tsv.o

clean:
	rm -fv De-MetaST-BLAST *.o Table/BLASTxml_to_tsv Table/*.o
