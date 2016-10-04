/*
 * TadLista.h
 *
 *  Created on: 06/08/2013
 *      Author: matheus
 */

#ifndef TADLISTA_H_
#define TADLISTA_H_

#include "TadArvore.h"
#include "bitmap.h"

typedef struct lista ListaArv;

typedef struct no NoArv;

int lista_tam (ListaArv* l);

NoArv* lista_pri (ListaArv* l);

NoArv* lista_ult (ListaArv* l);

struct arv *noarv_arv (NoArv* n);

NoArv* noarv_prox (NoArv* n);

NoArv* noarv_ant (NoArv* n);

ListaArv* lista_criavazia (void);

ListaArv* lista_destroi (ListaArv* l);

NoArv* noarv_cria (struct arv *a);

NoArv* noarv_destroi (NoArv* n);

void lista_insere_crescente (NoArv* n, ListaArv* l);

NoArv* lista_remove_primeiro (ListaArv* l);

void lista_imprime (ListaArv* l);

void tradutor_huffman (bitmap *bm, unsigned char c, ListaArv* ListaFolhas, FILE *arq);

unsigned long long int lista_tamanho_arqComp(ListaArv* listaFolhas);

#endif /* TADLISTA_H_ */
