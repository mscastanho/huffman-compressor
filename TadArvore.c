/*
 * TadArvore.c
 *
 *  Created on: 06/08/2013
 *      Author: matheus
 */

#include <stdio.h>
#include <stdlib.h>

#include "bitmap.h"
#include "TadArvore.h"
#include "TadLista.h"
#include "math.h"

struct arv
{
	unsigned char item;
	unsigned long long int peso;
	unsigned char* codigo;
	int tamCod;
	struct arv *filhaDir;
	struct arv *filhaEsq;
};

/*
 *Cria uma nova árvore vaiza
 *inputs: nenhum
 *output: NULL
 *pré-condição: nenhuma
 *pós-condição: árvore inicializada com NULL
 */
Arv* arv_criavazia (void)
{
	return NULL;
}

/*
 *Cria uma nova árvore com item 'c', peso 'p', sub-árvore esqueda 'e' e sub-árvore direita 'd'
 *inputs: Um unsigned char 'c', um int 'p', uma árvore 'e' e uma árvore 'd'
 *output: a árvore inicializada
 *pré-condição: nenhuma
 *pós-condição: árvore aloca e com os campos preenchidos
 */
Arv* arv_cria (unsigned char c, unsigned long long int p, Arv* e, Arv* d)
{
	Arv *a = (Arv*) malloc (sizeof(Arv));

	a->item = c;
	a->peso = p;
	a->filhaDir = d;
	a->filhaEsq = e;
	a->tamCod = 0;
	a->codigo = NULL;

	return a;
}

/*
 *Aloca o espaço de memória de todos os campos 'código' das árvores com o tamanho 'tam'
 *inputs: a árvore que terá os códigos inicializados e o tamanho dos códigos 'tam'
 *output: nenhum
 *pré-condição: árvore não nula
 *pós-condição: árvore e sub-árvores com códigos inicializados de tamanhoa 'tam'
 */
void arv_inicia_codigos(Arv *a, int tam)
{
	int i;

	a->tamCod = tam;
	a->codigo = (unsigned char*) malloc (tam*sizeof(unsigned char));

	for(i=0;i<tam;i++)
		a->codigo[i] = 2;

	if (a->filhaDir != NULL && a->filhaEsq != NULL)
	{
		arv_inicia_codigos(a->filhaEsq,tam);
		arv_inicia_codigos(a->filhaDir,tam);
	}
}

/*
 *Libera o espaço alocado por uma árvore e suas sub-árvores
 *inputs: a árvore a ser liberada
 *output: NULL
 *pré-condição: nenhuma
 *pós-condição: espaço de memória utilizado pela árvore liberado
 */
Arv* arv_libera (Arv* a)
{
	if ( a!=NULL)
	{
		if ( a->filhaEsq == NULL && a->filhaDir == NULL)
		{
			free(a->codigo);
			free(a);

			a = NULL;
		}
		else
		{
			if(a->filhaEsq != NULL)
				arv_libera(a->filhaEsq);

			if(a->filhaDir != NULL)
				arv_libera(a->filhaDir);

			free(a->codigo);
			free(a);
			a = NULL;
		}
	}

	return a;
}

/*
 *Verifica se uma árvore está vazia
 *inputs: a árvore
 *output: 1 se estiver vazia e 0 caso contrário
 *pré-condição: nenhuma
 *pós-condição: sem alteração
 */
int arv_vazia (Arv* a)
{
	return a == NULL;
}

/*
 *Imprime uma árvore
 *inputs: a árvore a ser impressa
 *output: nenhum
 *pré-condição: nenhuma
 *pós-condição: sem alteração
 */
void arv_imprime (Arv* a)
{
	if ( a == NULL )
		printf("<>");
	else
	{
		printf("< %u ",a->item);
		arv_imprime(a->filhaEsq);
		printf(" ");
		arv_imprime(a->filhaDir);
		printf(" > ");
	}

}

/*
 *Retorna o valor do campo 'item' de uma árvore
 *inputs: a árvore
 *output: o unsigned char do campo 'item'
 *pré-condição: árvore não nula
 *pós-condição: sem alteração
 */
unsigned char arv_info (Arv* a)
{
	return a->item;
}

/*
 *Retorna o valor do campo 'peso' de uma árvore
 *inputs: a árvore
 *output: o int do campo 'item'
 *pré-condição: árvore não nula
 *pós-condição: sem alteração
 */
unsigned long long int arv_peso (Arv* a)
{
	return a->peso;
}

/*
 *Retorna o ponteiro para o campo 'código' de um árvore
 *inputs: a árvore
 *output: o ponteiro para o campo 'código'
 *pré-condição: árvore não nula
 *pós-condição: sem alteração
 */
unsigned char* arv_codigo (Arv* a)
{
	return a->codigo;
}

/*
 *Retorna o valor do campo 'codigo' na posicao 'index' de uma árvore 'a'
 *inputs: a árvore e o index
 *output: o unsigned char da posicao 'index' do codigo da arvore
 *pré-condição: árvore não nula e index menor que  a->tamCod
 *pós-condição: sem alteração
 */
unsigned char arv_codigo_info (Arv* a, int index)
{
	return a->codigo[index];
}

/*
 *Define um novo valor para o campo 'item' de uma árvore
 *inputs: a árvore e o novo item (unsigned char)
 *output: nenhum
 *pré-condição: nenhuma
 *pós-condição: a->item agora tem o valor 'c'
 */
void arv_set_info (Arv* a, unsigned char c)
{
	if ( a != NULL )
		a->item = c;
}

/*
 *Retorna a sub-árvore esquerda de uma árvore
 *inputs: a árvore
 *output: a sub-árvore esquerda
 *pré-condição: árvore não nula
 *pós-condição: sem alteração
 */
Arv* arv_filhaEsq(Arv* a)
{
	return a->filhaEsq;
}

/*
 *Retorna a sub-árvore direita de uma árvore
 *inputs: a árvore
 *output: a sub-árvore direita
 *pré-condição: árvore não nula
 *pós-condição: sem alteração
 */
Arv* arv_filhaDir(Arv* a)
{
	return a->filhaDir;
}

/*
 *Retorna o campo tamCod de uma árvore
 *inputs: a árvore
 *output: o valor do campo tamCod
 *pré-condição: árvore não nula
 *pós-condição: sem alteração
 */
int arv_tamCod (Arv* a)
{
		return a->tamCod;
}

/*
 *Retorna o maior entre dois inteiros 'a' e 'b'
 *inputs: 'a' e 'b'
 *output: o maior dos dois
 *pré-condição: nenhuma
 *pós-condição: nenhuma
 */
static int maior (int a, int b)
{
	return (a>b) ? a : b;
}

/*
 *Calcula a altura de uma árvore. Retorna '0' para árvore sem filhos.
 *inputs: a árvore
 *output: a altura da árvore
 *pré-condição: nenhuma
 *pós-condição: sem alteração
 */
int arv_altura (Arv* arv)
{
	if (arv != NULL)
	{
		if (arv->filhaEsq == NULL && arv->filhaDir == NULL)
			return 0;
		else
		{
			return maior (1+arv_altura(arv->filhaDir),1+arv_altura(arv->filhaEsq));
		}
	}

	return 0;
}

/*
 *Verifica se uma árvore é folha ou não
 *inputs: a árvore
 *output: 1 se for folha, 0 caso contrário
 *pré-condição: nenhum
 *pós-condição: sem alteração
 */
int arv_folha (Arv* a)
{
	if (a == NULL)
		return 0;
	else
		return a->filhaEsq == NULL && a->filhaDir == NULL;
}

/*
 *Monta a árvore de huffman baseada numa lista de árvores
 *inputs: a lista de árvores
 *output: a árvore de huffman
 *pré-condição: lista não nula
 *pós-condição: lista vazia e árvore de huffman montada
 */
Arv* arv_huffman (ListaArv* l)
{
	Arv  *a, *arv_esq , *arv_dir , *huffman;
	NoArv *n, *no_esq, *no_dir, *NoArvHuffman;

	huffman = NULL;


	if ( lista_tam(l) > 1)
	{
		do
		{
			no_esq = lista_remove_primeiro(l);
			no_dir = lista_remove_primeiro(l);

			arv_esq = noarv_arv(no_esq);
			arv_dir = noarv_arv(no_dir);

			noarv_destroi(no_esq);
			noarv_destroi(no_dir);

			// a árvore soma é criada com 0 no campo item
			a = arv_cria (0,arv_esq->peso + arv_dir->peso,arv_esq,arv_dir);

			n = noarv_cria(a);

			lista_insere_crescente(n,l);

		}while( lista_tam(l) > 1);

		NoArvHuffman = lista_remove_primeiro(l);
		huffman = noarv_arv ( NoArvHuffman);
		noarv_destroi(NoArvHuffman);

	}

	return huffman;
}

/*
 *Define os códigos baseados na árvore de huffman para todos os nós folha (0 para esquerda e 1 para direita)
 *inputs: a árvore de huffman
 *output: nenhum
 *pré-condição: árvore não nula
 *pós-condição: árvores com campos código preenchidos com o seu respectivo código
 */
void arv_codifica (Arv* huff)
{
	int i;

	if (huff->filhaDir != NULL && huff->filhaEsq != NULL)
	{
		for (i = 0; huff->codigo[i] != 2 ; i++) //Grava o código da árvore atual nas suas duas subárvores
		{
			huff->filhaDir->codigo[i] = huff->codigo[i];
			huff->filhaEsq->codigo[i] = huff->codigo[i];
		}


		huff->filhaDir->codigo[i] = 1;
		huff->filhaEsq->codigo[i] = 0;

		arv_codifica(huff->filhaEsq);
		arv_codifica(huff->filhaDir);
	}
}

/*
 *Recebe uma árvore de huffman, imprime no bitmap a árvore serializada e retorna o tamanho do cabecalho
 *inputs: a árvore de huffman, o bitmap e um arquivo para possível impressão (caso o bitmap esteja cheio)
 *output: o tamanho do cabeçalho
 *pré-condição: bitmap inicializado e arquivo não nulo
 *pós-condição: cabeçalho gravado no bitmap ou impresso no arquivo (caso o bitmap tenha que ter sido usado mais de uma vez)
 */
long int arv_serializa (Arv* huff, bitmap* bm, FILE* arq)
{
	//Convenção utilizada: '0' para nó não folha e '1' para folha
	long int c = 0;

	if (huff != NULL)
	{
		if (huff->filhaEsq == NULL && huff->filhaDir == NULL ) //se for folha
		{
			bm = bitmap_verifica(bm,arq);
			bitmapAppendLeastSignificantBit(bm,1);
			bitmap_escreve_char(bm,huff->item,arq);

			return c + 9;

		}
		else
		{
			bm = bitmap_verifica(bm,arq);
			bitmapAppendLeastSignificantBit(bm,0);

			return 1 + arv_serializa (huff->filhaEsq,bm,arq) + arv_serializa (huff->filhaDir,bm,arq);
		}
	}

	return 0;
}

/*
 *Cria uma lista de árvores com todos os nós folha de uma determinada árvore
 *inputs: a árvore e uma lista de árvores inicializada
 *output: nenhum
 *pré-condição: lista inicializada
 *pós-condição: lista contém todos os nós folha da árvore de entrada organizados por ordem crescente do peso
 */
void arv_insere_folhas_lista (Arv* a, ListaArv* l)
{
	if (a != NULL)
	{
		if (a->filhaDir == NULL && a->filhaEsq == NULL) //se for folha
		{
			NoArv *n = noarv_cria(a);
			lista_insere_crescente (n,l);
		}
		else
		{
			arv_insere_folhas_lista (a->filhaEsq,l);
			arv_insere_folhas_lista (a->filhaDir,l);
		}
	}

}

/*
 *Remonta a árvore de huffman a partir de um cabeçalho
 *inputs: um bitmap preenchido com uma árvore serializada
 *output: a árvore de huffman montada
 *pré-condição: o bitmap contém toda o cabeçalho (árvore serializada)
 *pós-condição: árvore de huffman montada.
 */
Arv* arv_deserializa (bitmap *bm)
{
	unsigned int posInicial = bitmapGetLengthAgregado(*bm);

	unsigned char bit = bitmapGetBit(*bm,posInicial);

	int c = 0;
	int i;
	Arv* a;
	Arv* b = NULL;

	bitmapAddLengthAgregado(bm,1);  // O número de bits já lidos será gravado
										// no campo LengthAgregado
	if ( bit == 1 )
	{
		for ( i = 1 ; i < 9 ; i++)
		{
			c += pow(2,8-i)*bitmapGetBit(*bm,posInicial+i);
		}

		bitmapAddLengthAgregado(bm,8);

		a = arv_cria(c,0,b,b);
	}
	else
	{
		Arv *filhaEsq = arv_deserializa (bm);
		Arv *filhaDir = arv_deserializa (bm);
		a = arv_cria(0,0,filhaEsq,filhaDir);
	}

	return a;

}
