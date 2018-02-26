#pareja 9

CC = gcc
EXE = ej4a ej4b ej5a ej5b ej6 ej8 ej9
FLAGS = -Wall -g 

all: $(EXE)

ej4a: ej4a.c
	$(CC) $(FLAGS) $< -o $@

ej4b: ej4b.c
	$(CC) $(FLAGS) $< -o $@

ej5a: ej5a.c
	$(CC) $(FLAGS) $< -o $@

ej5b: ej5b.c
	$(CC) $(FLAGS) $< -o $@

ej6: ej6.c
	$(CC) $(FLAGS) $< -o $@
	
ej8: ej8.c
	$(CC) $(FLAGS) $< -o $@
	
ej9: ej9.c
	$(CC) $(FLAGS) $< -o $@ -lm
	
PHONY: clean

clean:
	rm -f $(EXE) *.o

