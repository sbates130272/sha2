EXE=sha256_test

default:$(EXE)

sha256.o: sha256.c sha256.h
	$(CC) -c sha256.c

$(EXE): sha256_test.c sha256.o
	$(CC) -o sha256_test sha256_test.c sha256.o

clean:
	rm -rf $(EXE) *~ sha256.o
