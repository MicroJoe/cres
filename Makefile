EXEC=cres

all: main.c
	$(CC) main.c -o $(EXEC)

extractible: out.c
	$(CC) out.c -o extractible
