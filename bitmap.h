/** Define um TAD representando um mapa de bits.
 * @file bitmap.h
 * @author Joao Paulo Andrade Almeida (jpalmeida@inf.ufes.br)
 */

#ifndef BITMAP_H_
#define BITMAP_H_
#define TAM_BITMAP 262144 //8^6, bitmap cheio tem numero inteiro de bytes

/**
 * Estrutura para representar um mapa de bits.
 */
typedef struct {
	unsigned int max_size;		///< tamanho maximo em bits
	unsigned int length; 		///< tamanho atual em bits
	unsigned char* contents; 	///< conteudo do mapa de bits
	unsigned int lengthAgregado; //< tamanho agregado dos bitmaps anteriores
} bitmap;

unsigned char* bitmapGetContents(bitmap bm);
unsigned int bitmapGetMaxSize(bitmap bm);
unsigned int bitmapGetLength(bitmap bm);
bitmap* bitmapInit(unsigned int max_size);
unsigned char bitmapGetBit(bitmap bm, unsigned int index);
void bitmapSetBit(bitmap* bm, unsigned int index, unsigned char bit);
void bitmapAppendLeastSignificantBit(bitmap* bm, unsigned char bit);

// Funções adicionais -- por Matheus Salgueiro Castanho

unsigned int bitmapGetLengthAgregado(bitmap bm);

void bitmapAddLengthAgregado (bitmap *bm, unsigned int la);

void bitmapSetLengthAgregado (bitmap *bm, unsigned int la);

void bitmap_escreve_char (bitmap *bm, unsigned char c, FILE* arq);

void bitmap_escreve_arquivo (bitmap bm, FILE *arq);

void bitmap_destroi (bitmap *bm);

bitmap* bitmap_reinicializa (bitmap* bm);

int bitmap_cheio (bitmap* bm);

bitmap* bitmap_verifica (bitmap* bm, FILE *arq);

#endif /*BITMAP_H_*/
