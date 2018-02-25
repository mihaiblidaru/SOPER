EXE = ej4a ej4b ej5a ej5b ej6 ej8
FLAGS = -Wall

all: $(EXE)

ej4a: ej4a.c
	gcc $(FLAGS) $< -o $@

ej4b: ej4b.c
	gcc $(FLAGS) $< -o $@

ej5a: ej5a.c
	gcc $(FLAGS) $< -o $@

ej5b: ej5b.c
	gcc $(FLAGS) $< -o $@

ej6: ej6.c
	gcc $(FLAGS) $< -o $@
	
ej8: ej8.c
	gcc $(FLAGS) $< -o $@
	
PHONY: clean

clean:
	rm -f $(EXE) *.o

