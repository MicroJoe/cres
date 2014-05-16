EXEC=cres
CFLAGS=-Wall -Wextra

all: main.c
	$(CC) main.c -o $(EXEC) $(CFLAGS)

extractible: out.c
	$(CC) out.c -o extractible $(CFLAGS)
