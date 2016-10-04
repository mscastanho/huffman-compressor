/*
 * TadLista.c
 *
 *  Created on: 06/08/2013
 *      Author: matheus
 */
#include <stdio.h>
#include <stdlib.h>

#include "TadArvore.h"
#include "TadLista.h"

struct lista //lista duplamente encadeada
{
	NoArv* pri;
	NoArv* ult;
	int tam;
};

struct no
{
	Arv* arv;
	struct no *prox;
	struct no *ant;
};

// Funções de acesso da lista


/*
 *Retorna o tamanho de uma lista
 *inputs: a lista
 *output: o tamanho da lista
 *pré-condição: lista não nula
 *pós-condição: sem alteração
 */
int lista_tam (ListaArv* l)
{
	return l->tam;
}

/*
 *Retorna o primeiro nó de uma lista
 *inputs: a lista
 *output: o primeiro nó da lista
 *pré-condição: lista não nula
 *pós-condição: sem alteração
 */
NoArv* lista_pri (ListaArv* l)
{
	return l->pri;
}

/*
 *Retorna o último nó de uma lista
 *inputs: a lista
 *output: o último nó da lista
 *pré-condição: lista não nula
 *pós-condição: sem alteração
 */
NoArv* lista_ult (ListaArv* l)
{
	return l->ult;
}

//------------------------------//

// Funções de acesso do nó

/*
 *Retorna a árvore de um nó de lista de árvores
 *inputs: o nó de lista
 *output: a árvore correspondente
 *pré-condição: nó não nulo
 *pós-condição: sem alteração
 */
Arv* noarv_arv (NoArv* n)
{
	return n->arv;
}

/*
 *Retorna o próximo nó de uma lista de árvores
 *inputs: um nó de lista
 *output: o próximo nó
 *pré-condição: nó não nulo
 *pós-condição: sem alteração
 */
NoArv* noarv_prox (NoArv* n)
{
	return n->prox;
}

/*
 *Retorna o nó anterior de uma lista de árvores
 *inputs: um nó de lista
 *output: o nó anterior
 *pré-condição: nó não nulo
 *pós-condição: sem alteração
 */
NoArv* noarv_ant (NoArv* n)
{
	return n->ant;
}

//-----------------------------//

/*
 *Cria uma lista vazia
 *inputs: nenhum
 *output: lista inicializada e vazia
 *pré-condição: nenhuma
 *pós-condição: lista inicializada
 */
ListaArv* lista_criavazia (void)
{
	ListaArv* l = (ListaArv*) malloc (sizeof(ListaArv));
	l->pri = NULL;
	l->ult = NULL;
	l->tam = 0;

	return l;
}

/*
 *Libera o espaço de memória utilizado por uma lista de árvores
 *inputs: a lista
 *output: NULL
 *pré-condição: nenhuma
 *pós-condição: espaço de memória liberado
 */
ListaArv* lista_destroi (ListaArv* l)
{
	NoArv *aux, *marc;

	if (l != NULL)
	{
		for (marc = l->pri ; marc != NULL ; marc = aux )
		{
			aux = marc->prox;
			noarv_destroi(marc);
		}
	}

	free(l);
	l = NULL;
	return l;
}

/*
 *Cria um nó de lista de árvores com uma árvore 'a'
 *inputs: uma árvore 'a'
 *output: o nó inicilizado com a árvore 'a'
 *pré-condição: nenhuma
 *pós-condição: nó inicializado com prox e ant como NULL e arv apontando para a árvore 'a'
 */
NoArv* noarv_cria (Arv* a) //cria com prox e ant apontando pra null
{
	NoArv* n = (NoArv*) malloc (sizeof(NoArv));
	n->prox = NULL;
	n->ant = NULL;
	n->arv = a;

	return n;
}

/*
 *Libera o espaço de memória ocupado por um nó de lista de árvore
 *inputs: um nó de lista
 *output: NULL
 *pré-condição: nó não nulo
 *pós-condição: espaço de memória liberado
 */
NoArv* noarv_destroi (NoArv* n) // só deleta o nó, não deleta o seu conteúdo.
{
	free (n);
	n = NULL;
	return n;
}


/*
 *Insere ordenadamente um nó numa lista de árvores já ordenada crescentemente pelo peso
 *inputs: um nó de lista
 *output: nenhum
 *pré-condição: lista inicializada e ordenada crescentemente pelo peso
 *pós-condição: item inserido em ordem crescente do peso
 */
void lista_insere_crescente (NoArv* n, ListaArv* l) // lista de entrada != NULL / peso  crescente
{
	NoArv *marc;

	if (l->tam == 0)
	{
		l->pri = l->ult = n;
	}
	else
	{
		//Procura o primeiro item que tem um peso maior do que o nó a ser inserido
		for (marc = l->pri ; marc != NULL && arv_peso(marc->arv) <= arv_peso(n->arv) ; marc = marc->prox);

		if (marc == NULL) 			// O peso do nó a ser inserido é
		{							// maior do que todos na lista.
			l->ult->prox = n;		// O elemento será inserido no
			n->ant = l->ult;		// final da lista.
			l->ult = n;
		}
		else
		{
			if (marc->ant == NULL)      // O peso do nó a ser inserido é
			{							// menor do que todos na lista.
				l->pri->ant = n;		// O elemento será inserido no
				n->prox = l->pri;		// começo da lista.
				l->pri = n;
			}
			else // O elemento será inserido no meio da lista, imediatamente antes de marc
			{
				marc->ant->prox = n;
				n->ant = marc->ant;

				marc->ant = n;
				n->prox = marc;

			}
		}
	}

	l->tam++;
}

/*
 *Remove o primeiro nó de uma lista de árvores
 *inputs: uma lista de árvores
 *output: o nó removido
 *pré-condição: lista não nula
 *pós-condição: primeiro nó retirado e tamanho da lista diminuído de 1
 */
NoArv* lista_remove_primeiro (ListaArv* l) // Retira o primeiro elemento da lista
{											  // pré-condição: lista não nula
	NoArv* aux = NULL;

	if (l->tam != 0) // caso a lista esteja vazia, a função não faz nada
	{
		if (l->tam == 1)
		{
			aux = l->pri;
			l->pri = NULL;
			l->ult = NULL;

		}
		else
		{
			aux = l->pri;
			l->pri->prox->ant = NULL;
			l->pri = l->pri->prox;
		}

		l->tam--;
	}

	return aux;
}

/*
 *Imprime uma lista de árvores
 *inputs: a lista
 *output: nenhum
 *pré-condição: lista não nula
 *pós-condição: sem alteração
 */
void lista_imprime (ListaArv* l)
{
	NoArv *marc;
	int i;

	if (l->tam != 0)
	{
		for ( marc = l->pri ; marc != NULL ; marc = marc->prox )
		{
			printf("Item: %u\nPeso: %Ld\nTamanho do Código: %d\nCódigo: ",arv_info(marc->arv),arv_peso(marc->arv),arv_tamCod(marc->arv));

			for(i = 0 ; arv_codigo(marc->arv)[i] != 2 && i < arv_tamCod(marc->arv) ; i++)
				printf("%u", arv_codigo(marc->arv)[i]);
			printf("\n\n");

		}
	}
	else
		printf("ERRO: Lista vazia!\n");
}

/*
 *Traduz um unsigned char 'c' de acordo com uma lista de folhas, e grava a a tradução (caracter compactado) no bitmap.
 *se o bitmap estiver cheio, a função imprime toda a informação no arquivo 'arq', reinicializa o bitmap e continua a tradução.
 *inputs: o bitmap para escrita, o caracter a ser traduzido (unsigned char), uma lista de folhas de uma árvore de huffman e um arquivo de saída.
 *output: nenhum
 *pré-condição: arquivo e bitmap não nulos e todas as árvores da lista de folhas com código inicializado
 *pós-condição: caracter traduzido escrito no bitmap
 */
void tradutor_huffman (bitmap *bm, unsigned char c, ListaArv* ListaFolhas, FILE* arq)
{
	int j;
	NoArv* marc;

		//procura o caracter na lista de folhas (de trás pra frente, para melhor desempenho)
		for(marc = ListaFolhas->ult ; marc != NULL && arv_info(marc->arv) != c; marc = marc->ant);

		//Escreve o codigo correspondente no bitmap
		for (j = 0 ; j < arv_tamCod(marc->arv) && arv_codigo(marc->arv)[j] != 2 /*&& j < arv_tamCod(marc->arv)*/ ; j++)
		{
			bm = bitmap_verifica(bm,arq);
			bitmapAppendLeastSignificantBit(bm, arv_codigo(marc->arv)[j] );
		}
}

/*
 *Retorna o tamanho (em bits) de toda a informação compactada
 *inputs: uma lista de folhas da árvore de huffman do arquivo de origem
 *output: o tamanho da informação compactada (em bits)
 *pré-condição: todas as árvores da lista de folhas com código inicializado
 *pós-condição: item inserido em ordem crescente do peso
 */
unsigned long long int lista_tamanho_arqComp(ListaArv* listaFolhas)
{
	int j;
	NoArv* marc;
	unsigned long long int somaBits = 0;

	for (marc = listaFolhas->pri ; marc != NULL ; marc = marc->prox)
	{
		if (arv_peso(marc->arv) != 0)
		{
			for (j = 0; j < arv_tamCod(marc->arv) && arv_codigo_info(marc->arv,j) != 2 ; j++);

			somaBits += j*arv_peso(marc->arv);
		}
	}

	return somaBits;
}
