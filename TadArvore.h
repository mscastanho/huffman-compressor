/*
 * TadArvore.h
 *
 *  Created on: 06/08/2013
 *      Author: matheus
 */

#ifndef TADARVORE_H_
#define TADARVORE_H_

#include "TadLista.h"
#include "bitmap.h"

typedef struct arv Arv;

Arv* arv_criavazia (void);

Arv* arv_cria (unsigned char c, unsigned long long int p, Arv* e, Arv* d);

void arv_inicia_codigos(Arv *a, int tam);

Arv* arv_libera (Arv* a);

int arv_vazia (Arv* a);

void arv_imprime (Arv* a);

unsigned char arv_info (Arv* a);

unsigned long long int arv_peso (Arv* a);

unsigned char* arv_codigo (Arv* arv);

unsigned char arv_codigo_info (Arv* a, int index);

Arv* arv_filhaEsq(Arv* a);

Arv* arv_filhaDir(Arv* a);

int arv_tamCod (Arv* a);

int arv_altura (Arv* arv);

int arv_folha (Arv* a);

void arv_codifica (Arv* huff);

long int arv_serializa (Arv* huff, bitmap* bm, FILE* arq);

Arv* arv_huffman (ListaArv* l);

void arv_insere_folhas_lista (Arv* a, ListaArv* l);

Arv* arv_deserializa (bitmap *bm);

#endif /* TADARVORE_H_ */
