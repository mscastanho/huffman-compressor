/*
 * Descompactador.c
 *
 *  Created on: 06/09/2013
 *      Author: matheus
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TadArvore.h"
#include "TadLista.h"
#include "bitmap.h"

/*
 *Descompactada (destraduz) a informação comapactada armazenada em um bitmap e escreve num arquivo de saída (aberto dentro da função)
 *inputs: o bitmap com parte ou toda a informação a ser descompactada, o tamanho da informação compactada,
 *        o tamanho do nome do arquivo de origem, a árvore de huffman do arquivo, e o arquivo compactado (para leitura)
 *output: nenhum
 *pré-condição: árvore de huffman não nula, arqE não nulo, bitmap não nulo e tamanhos corretos
 *pós-condição: arquivo de saída corresponde ao arquivo descompactado, com o mesmo nome e informação do original.
 */

void DesTraduz (bitmap *bm, unsigned long long int tamInfo, int tamNome, Arv* huffman, FILE* arqE)
{
	unsigned long long int caracteresDesTraduzidos = 0;
	unsigned long long int contador = 0;
	unsigned char bit;
	Arv* a = huffman;
	unsigned long long int tamOrigem;
	unsigned long long int i;
	unsigned char c;
	unsigned long int proxPos = 0;
	FILE *arqS;

	char* nomeArquivo = (char*) malloc ((tamNome+1)*sizeof(char));    //Espaço alocado para o nome do arquivo de saída

	tamOrigem = bitmapGetLength(*bm);									//Até onde o bitmap inicial tem informação útil

	while ( contador < tamInfo)
	{
		while ( a != NULL && arv_folha(a) != 1 && contador < tamInfo ) //Lê a informação compactada e vai andando pela árvore
		{																//até decodificar a informação

			if ( proxPos == tamOrigem )									//Caso todo o bitmap já tenha sido traduzido
			{															//e ainda exista informação no arquivo compactado a ser traduzida,
				bm = bitmap_reinicializa(bm);							//há a reinicialização do bitmap com mais informação a descompactar

				for(i = 0 ; i < TAM_BITMAP/8 ; i++)
				{
					fread(&c,sizeof(unsigned char),1,arqE);           //Enche o bitmap novamente
					bitmap_escreve_char(bm,c,NULL);					    //com informações do arquivo compactado
				}

				proxPos = 0;
				tamOrigem = TAM_BITMAP;
			}

				bit = bitmapGetBit(*bm,proxPos);

			if (bit == 0)
				a = arv_filhaEsq(a);
			else
				a = arv_filhaDir(a);

			proxPos++;
			contador++;
		}

		unsigned char c = arv_info(a);

		caracteresDesTraduzidos++;

		if (caracteresDesTraduzidos <= tamNome)				//Os primeiros tamNome caracteres a serem destraduzidos são referentes
		{													//ao nome do arquivo de origem
			char d = c;
			nomeArquivo[caracteresDesTraduzidos-1] = d;
		}
		else
		{
			if (caracteresDesTraduzidos == tamNome+1)
			{
				nomeArquivo[tamNome] = '\0';				//Aqui o arquivo de saída é aberto, pois só nesse ponto
				arqS = fopen(nomeArquivo,"wb");				//se saberá o seu devido nome (nome do arquivo original)
			}

			fwrite(&c,sizeof(unsigned char),1,arqS);		//Caracter descompactado é escrito no arquivo de saída.

		}

		a = huffman;
	}

	fclose(arqS);											//Fechamento do arquivo de saída.
	free(nomeArquivo);										//Liberação do espaço alocado para o nome do arquivo
}

int main (int argc, char *argv[])
{
	FILE *arqE;
	unsigned int tamCab;
	unsigned long long int tamInfo;
	unsigned char c;
	unsigned long long int contador = 0;
	unsigned long long int i;
	int tamNome;
	Arv* huffman;

	arqE = fopen(argv[1],"rb");

	if ( arqE == NULL)
	{
		printf("ERRO: Arquivo inexistente. Tente Novamente.\n\n");
		return 0;
	}
	//Leitura do tamanho do cabeçalho, da informação compactada
	//e do tamanho do nome do arquivo original

	fread(&tamCab,sizeof(unsigned int),1,arqE);
	fread(&tamInfo,sizeof(unsigned long long int),1,arqE);
	fread(&tamNome,sizeof(int),1,arqE);

	/////////////////////////////////////////////////////////////////


	//Leitura do cabeçalho e remontagem da árvore de huffman

	bitmap *bmCab = bitmapInit(tamCab+7);

	do
	{
		fread(&c,sizeof(unsigned char),1,arqE);		//Todo o cabecalho será adicionado ao bitmap, juntamente com um pedaço
		bitmap_escreve_char(bmCab,c,NULL);              //do nome do arquivo original (menos de 1 byte)
		contador++;

	}while(contador != (tamCab+7)/8);

	bitmapSetLengthAgregado(bmCab,0);

	huffman = arv_deserializa(bmCab);                   // Aqui a árvore de huffman é remontada a partir do cabeçalho.

	/////////////////////////////////////////////////////////////////


	int bitsSobrando = ((tamCab+7)/8)*8 - tamCab; 		//Número de bits do nome do arquivo que
														//foram lidos juntamente com o cabeçalho.

	bitmap *bm = bitmapInit(TAM_BITMAP + bitsSobrando);

	unsigned char bit;

	for ( i = 0 ; i < bitsSobrando ; i++ )
	{
		bit = bitmapGetBit(*bmCab,tamCab+i);			//Passa os bits excedentes do bitmap do cabeçalho
		bitmapAppendLeastSignificantBit(bm,bit);    	//para o bitmap da informação compactada
	}


	unsigned long long int tamInfoBytes = (tamInfo+7-bitsSobrando)/8;
	contador = 0;

	do
	{
		fread(&c,sizeof(unsigned char),1,arqE);						//Aqui o bitmap é preenchido completamente com informação compactada
		bitmap_escreve_char(bm,c,NULL);									//que será destraduziada pela função DesTraduz.
		contador++;

	}while(contador != tamInfoBytes && contador != TAM_BITMAP/8 );

	DesTraduz(bm,tamInfo,tamNome,huffman,arqE);							//Caso ainda tenha mais informação a ser destraduzida, a própria função DesTraduz
																		//fará a leitura do arquivo e preenchimento do bitmap até que toda a informação
																		//seja destraduzida.

	//Liberação do espaço de memória alocado durante a execução do programa

	bitmap_destroi(bmCab);
	bitmap_destroi(bm);
	arv_libera(huffman);
	fclose(arqE);

	/////////////////////////////////////////////////////////////////

	return 0;
}



