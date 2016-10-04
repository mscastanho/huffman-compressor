all: compila

compila:
		gcc -c TadLista.c
		gcc -c TadArvore.c -lm
		gcc -c bitmap.c
		gcc Compactador.c -o Compactador TadArvore.o TadLista.o bitmap.o -lm
		gcc Descompactador.c -o Descompactador TadArvore.o TadLista.o bitmap.o -lm

