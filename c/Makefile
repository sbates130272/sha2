EXE=sha256_test

default:$(EXE)

argconfig.o: argconfig.c argconfig.h
	$(CC) -c argconfig.c

suffix.o: suffix.c suffix.h
	$(CC) -c suffix.c

sha256.o: sha256.c sha256.h
	$(CC) -c sha256.c

$(EXE): sha256_test.c sha256.o argconfig.o suffix.o
	$(CC) -o sha256_test sha256_test.c sha256.o argconfig.o suffix.o

clean:
	rm -rf $(EXE) *~ sha256.o
