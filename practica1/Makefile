# Práctica 1
# Sistemas Operativos - 2262
# 
# Pareja 9:
#        Lucia Fuentes
#        Mihai Blidaru
# 

CC = gcc
EXE = ejercicio4a ejercicio4b ejercicio5a ejercicio5b ejercicio6 ejercicio8 ejercicio9  ejercicio12a ejercicio12b ejercicio13
FLAGS = -Wall -g 

all: $(EXE)

ejercicio4a: ejercicio4a.c
	$(CC) $(FLAGS) $< -o $@

ejercicio4b: ejercicio4b.c
	$(CC) $(FLAGS) $< -o $@

ejercicio5a: ejercicio5a.c
	$(CC) $(FLAGS) $< -o $@

ejercicio5b: ejercicio5b.c
	$(CC) $(FLAGS) $< -o $@

ejercicio6: ejercicio6.c
	$(CC) $(FLAGS) $< -o $@
	
ejercicio8: ejercicio8.c
	$(CC) $(FLAGS) $< -o $@
	
ejercicio9: ejercicio9.c
	$(CC) $(FLAGS) $< -o $@ -lm

ejercicio12a: ejercicio12a.c
	$(CC) $(FLAGS) $< -o $@ -pthread -lm
	
ejercicio12b: ejercicio12b.c
	$(CC) $(FLAGS) $< -o $@ -pthread -lm
	
ejercicio13: ejercicio13.c
	$(CC) $(FLAGS) $< -o $@ -pthread -lm

PHONY: clean

clean:
	rm -f $(EXE) *.o
	rm -r -f doc/
	
doxygen:
	doxygen Doxyfile
