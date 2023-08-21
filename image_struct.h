// STEFAN MIRUNA ANDREEA 314CA

#pragma once

typedef struct image {
	// magic word-ul, care poate fi P2, P3, P5 sau P6
	char type[3];
	// latimea si inaltimea imaginii
	int width, height;
	// valoarea maxima a pixelilor din imagine
	int max_value;
	// matricea de pixeli pentru imaginile grayscale
	double **gray;
	// matricea de pixeli pentru imaginile RGB
	double **red, **green, **blue;
	// (0) imagine grayscale sau (1) imagine RGB
	int color;
	// selectia curenta
	int x1, y1, x2, y2;
	// contor care verifica daca exista vreo imagine incarcata
	// (0) imagine incarcata, (1) nicio imagine incarcata
	int loaded;
} image;
