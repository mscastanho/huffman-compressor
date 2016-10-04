/*
 * Compactador.c
 *
 *  Created on: 17/08/2013
 *      Author: matheus
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TadArvore.h"
#include "TadLista.h"
#include "bitmap.h"

#define TAM_BUFFER 5000
#define TAM 256

int main (int argc, char *argv[])
{

	FILE *arqE, *arqS;
	unsigned char* buffer;
	unsigned long long int repeticoes[TAM], i, j, contador;
	unsigned long long int tamArq = 0;
	unsigned long long int tamArqComp = 0;
	int tamNome = strlen(argv[1]);
	int pos;

	arqE = fopen(argv[1],"rb");

	if ( arqE == NULL)
	{
		printf("ERRO: Arquivo inexistente. Tente Novamente.\n\n");
		return 0;
	}

	// Determinacao do nome do arquivo compactado. Ex: entrada.txt --> entrada.comp

	char* nomeCpt = (char*) malloc ((tamNome+6)*sizeof(char));

	strcpy(nomeCpt,argv[1]);

	for (i = 0 ; i < strlen(nomeCpt) && nomeCpt[i] != '.' ; i++);

	nomeCpt[i] = '.';
	nomeCpt[i+1] = 'c';
	nomeCpt[i+2] = 'o';
	nomeCpt[i+3] = 'm';
	nomeCpt[i+4] = 'p';
	nomeCpt[i+5] = '\0';

	/////////////////////////////////////////////////////////////////

	for(i=0;i<TAM;i++) //vetor de repeticoes dos caracteres da tabela ascii
			repeticoes[i] = 0;

	arqS = fopen(nomeCpt,"wb");

	//Descobre o tamanho do arquivo
	fseek(arqE,0,SEEK_END); 	//posiciona o cursor no final do arquivo
	tamArq = ftell(arqE); 		//Tamanho do arquivo em bytes
	rewind(arqE); 				//volta o cursor para o início do arquivo

	buffer = (unsigned char*) malloc (TAM_BUFFER*sizeof(unsigned char)); //vetor para guardar as informações do arquivo
																			 //a serem codificadas

	//Preenchimento do vetor de frequências

	contador = 0;

	for ( i = 0 ; i < tamNome ; i++ )
	{
		pos = argv[1][i];
		repeticoes[ pos ]++; //conta o número de repetições de cada caracter do nome do arquivo
	}

	do
	{
		i = 0;

		do
		{
			fread (&buffer[i],sizeof(unsigned char),1,arqE);
			pos = buffer[i];
			repeticoes[ pos ]++; //conta o número de repetições de cada caracter do arquivo
			i++;

			contador++;
		}while( i < TAM_BUFFER && contador != tamArq);

	}while( contador != tamArq );

	/////////////////////////////////////////////////////////////////

	//Criação da árvore de huffman a partir do vetor de frequências

	ListaArv* l = lista_criavazia();

	for(j=0;j<TAM;j++)
	{

		if (repeticoes[j] != 0)
		{
			unsigned char c = j;
			Arv* a = arv_cria (c,repeticoes[j],NULL,NULL);
			NoArv* n = noarv_cria(a);
			lista_insere_crescente(n,l);
		}
	}

	Arv* huffman = arv_huffman(l);

	/////////////////////////////////////////////////////////////////

	//Alocação dos campos "código" das árvores, que terá exatamente o tamanho
	//do seu código da árvore de huffman

	int altura = arv_altura(huffman);

	arv_inicia_codigos(huffman, altura);

	arv_codifica(huffman);

	/////////////////////////////////////////////////////////////////

	bitmap *bm = bitmapInit(TAM_BITMAP);

	//Organizacao do arquivo compactado:
	//								   sizeof(unsigned int) bits	-----> Tamanho do cabeçalho
	//						 sizeof(unsigned long long int) bits	-----> Tamanho da informação compactada
	//									        sizeof(int) bits	-----> Tamanho da string do nome do arquivo (número de caracteres)
	//                       		           tamCabecalho bits	-----> Cabeçalho
	//									         tamArqComp bits	-----> Informação compactada


	unsigned int tamCabecalho = arv_serializa(huffman,bm,arqS);				// <------ Aqui o cabeçalho será gravado no bitmap

	fwrite(&tamCabecalho,sizeof(unsigned int),1,arqS);     // Tamanho do cabeçalho escrito no arquivo

	ListaArv* lf = lista_criavazia();

	arv_insere_folhas_lista (huffman,lf);                    // Criação da lista com as folhas da árvore de huffman

	rewind(arqE);

	contador = 0;

	//Inicio da tradução da informação para o arquivo compactado

	for ( i = 0 ; i < strlen(argv[1]) ; i ++ )              // O nome do arquivo é compactado logo após o cabeçalho,
		tradutor_huffman(bm,argv[1][i],lf,arqS);			// antes da informação compactada

	tamArqComp = lista_tamanho_arqComp(lf);					// Definição do tamanho do arquivo compactado

	fwrite(&tamArqComp,sizeof(unsigned long long int),1,arqS);            // Tamanho do arquivo compactado escrito no arquivo.
	fwrite(&tamNome,sizeof(int),1,arqS);									 // Tamanho do nome escrito no arquivo.

	do
	{
		i = 0;

		do
		{
			fread (&buffer[i],sizeof(unsigned char),1,arqE);                //Aqui toda a informação começa a ser traduzida para o bitmap.
			tradutor_huffman(bm,buffer[i],lf,arqS);							  //Quando o bitmap fica cheio, a informação é toda escrita no arquivo compactado
			i++;															  //e o processo se repete até que toda a informação tenha sido traduzida e escrita
			contador++;														  //no arquivo compactado.
		}while( i < TAM_BUFFER && contador != tamArq);

	}while( contador != tamArq );

	unsigned long long int rep = (bitmapGetLength(*bm)+7)/8;				  //Informação restante no bitmap que ainda não foi escrita,
	fwrite(bitmapGetContents(*bm),sizeof(unsigned char),rep,arqS);		  //agora é escrita no arquivo.

	/////////////////////////////////////////////////////////////////

	//Liberação de todo o espaço de memória alocado durante o programa

	lista_destroi(lf);
	lista_destroi(l);
	bitmap_destroi(bm);
	arv_libera(huffman);
	free(buffer);
	free(nomeCpt);
	fclose(arqE);
	fclose(arqS);

	/////////////////////////////////////////////////////////////////

	return 0;
}


