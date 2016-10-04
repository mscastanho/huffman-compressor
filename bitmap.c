/** Define um TAD representando um mapa de bits.
 * @file bitmap.c
 * @author Joao Paulo Andrade Almeida (jpalmeida@inf.ufes.br)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitmap.h"


/**
 * Funcao auxiliar que imprime uma mensagem de falha e aborta o programa caso testresult seja falso.
 * @param testresult Valor booleano representando o resultado do teste que deveria ser verdadeiro.
 * @param message A mensagem para ser impressa se resultado do teste for falso.
 */
void assert(int testresult, char* message) {
	if (!testresult) {
		printf("%s\n", message);
		exit(EXIT_FAILURE);
	}
}


/**
 * Retorna o conteudo do mapa de bits.
 * @param bm O mapa de bits.
 */
unsigned char* bitmapGetContents(bitmap bm) {
	return bm.contents;
}

/**
 * Retorna o tamanho maximo do mapa de bits.
 * @param bm O mapa de bits.
 * @return O tamanho maximo do mapa de bits.
 */
unsigned int bitmapGetMaxSize(bitmap bm) {
	return bm.max_size;
}

/**
 * Retorna o tamanho atual do mapa de bits.
 * @param bm O mapa de bits.
 * @return O tamanho atual do mapa de bits.
 */
unsigned int bitmapGetLength(bitmap bm) {
	return bm.length;
}

/**
 * Constroi um novo mapa de bits, definindo um tamanho maximo.
 * @param max_size O tamanho maximo para o mapa de bits.
 * @return O mapa de bits inicializado.
 */
bitmap* bitmapInit(unsigned int max_size) {
	bitmap* bm = (bitmap*) malloc (sizeof(bitmap));
	// definir tamanho maximo em bytes, com arredondamento para cima
	unsigned int max_sizeInBytes=(max_size+7)/8;
	// alocar espaco de memoria para o tamanho maximo em bytes
	bm->contents=calloc(max_sizeInBytes, sizeof(char));
	// verificar alocacao de memoria
	assert(bm->contents!=NULL, "Erro de alocacao de memoria.");
	// definir valores iniciais para tamanho maximo e tamanho atual
	bm->max_size=max_size;
	bm->length=0;
	bm->lengthAgregado = 0;
	return bm;
}

/**
 * Retorna o valor do bit na posicao index.
 * @param bm O mapa de bits.
 * @param index A posicao do bit.
 * @pre index<bitmapGetLength(bm)
 * @return O valor do bit.
 */
unsigned char bitmapGetBit(bitmap bm, unsigned int index) // index in bits
{
	// verificar se index<bm.length, pois caso contrario, index e' invalido
	assert(index<bm.length, "Acesso a posicao inexistente no mapa de bits.");
	// index/8 e' o indice do byte que contem o bit em questao
	// 7-(index%8) e' o deslocamento do bit em questao no byte
	return (bm.contents[index/8] >> (7-(index%8))) & 0x01;
}

/**
 * Modifica o valor do bit na posicao index.
 * @param bm O mapa de bits.
 * @param index A posicao do bit.
 * @param bit O novo valor do bit.
 * @post bitmapGetBit(bm,index)==bit
 */
void bitmapSetBit(bitmap* bm, unsigned int index, unsigned char bit) {
	// verificar se index<bm->length, pois caso contrario, index e' invalido
	assert(index<bm->length, "Acesso a posicao inexistente no mapa de bits.");
	// index/8 e' o indice do byte que contem o bit em questao
	// 7-(index%8) e' o deslocamento do bit em questao no byte
	bit=bit & 0x01;
	bit=bit<<(7-(index%8));
	bm->contents[index/8]= bm->contents[index/8] | bit;
}

/**
 * Adiciona um bit no final do mapa de bits.
 * @param bm O mapa de bits.
 * @param bit O novo valor do bit.
 * @pre bitmapGetLength(bm) < bitmapGetMaxSize(bm)
 * @post (bitmapGetBit(bm,bitmapGetLength(bm) @ pre)==bit)
 * and (bitmapGetLength(bm) == bitmapGetLength(bm) @ pre+1)
 */
void bitmapAppendLeastSignificantBit(bitmap* bm, unsigned char bit) {
	// verificar se bm->length<bm->max_size, caso contrario, o bitmap esta' cheio
	assert(bm->length<bm->max_size, "Tamanho maximo excedido no mapa de bits.");
	// como um bit sera' adicionado, devemos incrementar o tamanho do mapa de bits
	bm->length++;
	bitmapSetBit(bm, bm->length-1, bit);
}

// Funções adicionais -- por Matheus Salgueiro Castanho

/*
 *Retonar o campo LengthAgregado de um bitmap
 *inputs: o bitmap
 *output: o valor do campo LengthAgregado
 *pré-condição: bitmap não nulo
 *pós-condição: sem alteração
 */
unsigned int bitmapGetLengthAgregado(bitmap bm)
{
	return bm.lengthAgregado;
}

/*
 *Incrementa o campo LengthAgregado de um bitmap de um valor la
 *inputs: o bitmap e o incremento
 *output: nenhum
 *pré-condição: bitmap não nulo
 *pós-condição: campo LengthAgregado incrementado de la
 */
void bitmapAddLengthAgregado (bitmap *bm, unsigned int la)
{
	bm->lengthAgregado += la;
}

/*
 *Set o campo LengthAgregado de um bitmap
 *inputs: o bitmap e o novo valor do campo LengthAgregado
 *output: nenhum
 *pré-condição: bitmap não nulo
 *pós-condição: campo LengthAgregado tem o valor n
 */
void bitmapSetLengthAgregado (bitmap *bm, unsigned int n)
{
	bm->lengthAgregado = n;
}

/*
 *Escreve um unsigned char (em bits) em um bitmap
 *inputs: o bitmap, o unsigned char a ser escrito e um arquivo para impressão da informação caso o bitmap esteja cheio
 *output: nenhum
 *pré-condição: bitmap não nulo
 *pós-condição: unsigned char escrito no bitmap
 */
void bitmap_escreve_char (bitmap *bm, unsigned char c, FILE* arq)
{
	unsigned char tab[8];

	int i, divisao, resto, pos;

	for (i = 0 ; i < 8 ; i++ )
		tab[i] = 0;

	pos = 7; //ultima posicao do vetor ascii

	if (c != 0)
	{
		divisao = c;

		do
		{
			resto = divisao%2;
			divisao /= 2;

			if (resto == 1)
				tab[pos] = 1;
			else
				tab[pos] = 0;

			pos--;

		}while(divisao != 0);
	}

	for (i=0;i<8;i++)
	{
		bitmap_verifica(bm,arq);
		bitmapAppendLeastSignificantBit(bm,tab[i]);
	}
}

/*
 *Escreve toda a informação de um bitmap no arquivo de saída
 *inputs: o bitmap e o arquivo para impressao
 *output: nenhum
 *pré-condição: bitmap não nulo e arquivo não nulo
 *pós-condição: informação escrita no arquivo de saída.
 */
void bitmap_escreve_arquivo (bitmap bm, FILE *arq)
{
	long int rep = (bitmapGetLength(bm)+7)/8;
	fwrite(bitmapGetContents(bm),sizeof(unsigned char),rep,arq);
}

/*
 *Libera o espaço alocado por um bitmap
 *inputs: o bitmap
 *output: nenhum
 *pré-condição: bitmap não nulo
 *pós-condição: espaço de memória liberado.
 */
void bitmap_destroi (bitmap *bm)
{
	free(bm->contents);
	free(bm);
}

/*
 *Deleta um bitmap e retorna um novo bitmap
 *inputs: o bitmap a ser deletado
 *output: o endereço do novo bitmap alocado
 *pré-condição: bitmap não nulo
 *pós-condição: bitmap antigo deletado e novo bitmap inicializado
 */
bitmap* bitmap_reinicializa (bitmap* bm)
{
	bitmap* newBM;

	bitmap_destroi(bm);

	newBM = bitmapInit(TAM_BITMAP);

	return newBM;
}

/*
 *Verifica se um bitmap está cheio
 *inputs: o bitmap
 *output: 1 se estiver cheio 0 caso contrário
 *pré-condição: bitmap não nulo
 *pós-condição: sem alteração
 */
int bitmap_cheio (bitmap* bm)
{
	return bitmapGetMaxSize(*bm) == bitmapGetLength(*bm);
}

/*
 *Verifica se o bitmap está cheio. Caso sim, desaloca a memória alocada, imprime toda a informação em um arquivo e cria um outro bitmap no lugar
 *inputs: o bitmap a ser verificado e o arquivo de saída para impressão
 *output: o endereço do novo bitmap alocado
 *pré-condição: bitmap e arquivo de saída não nulos
 *pós-condição: se o bitmap não estiver cheio não há alteração. Caso contrário, informação é impressa no arquivo de saída, bitmap antigo desalocado e novo bitmap criado
 */
bitmap* bitmap_verifica (bitmap* bm, FILE *arq)
{
	unsigned int tamAntigo = bitmapGetLength(*bm) + bitmapGetLengthAgregado(*bm);

	if ( bitmap_cheio(bm) )
	{
			bitmap_escreve_arquivo(*bm,arq);
			bm = bitmap_reinicializa(bm);
			bm->lengthAgregado = tamAntigo;
	}

	return bm;
}
