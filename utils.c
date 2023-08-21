// STEFAN MIRUNA ANDREEA 314CA
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "image_struct.h"
#include "utils.h"

#define MAX 25
#define FREQ_MAX 256
#define MAX_PARAMETER 25
#define MAX_COMMAND 100
#define MAX_FILENAME 100

// functie pentru ignorarea comentariilor
void skip_comm(FILE *in)
{
	char ch;
	fscanf(in, "%c", &ch);
	if (ch == '\n')
		fscanf(in, "%c", &ch);
	if (ch == '#')
		do {
			fscanf(in, "%c", &ch);
		} while (ch != '\n');
	else
		fseek(in, -1, SEEK_CUR);
}

//programare defensiva
void defensive_programming(double **a)
{
	if (!a) {
		fprintf(stderr, "Failed to allocate memory");
		exit(0);
	}
}

// functie care aloca memorie pentru o matrice
double **alloc_matrix(int width, int height)
{
	double **a;
	a = (double **)malloc(height * sizeof(double *));
	for (int i = 0; i < height; i++)
		a[i] = (double *)malloc(width * sizeof(double));
	defensive_programming(a);
	return a;
}

//functia aloca memorie pentru matricea pixelilor unei imagini grayscale
void alloc_gray(image *photo)
{
	photo->gray = alloc_matrix(photo->width, photo->height);
}

//functia aloca memorie pentru matricea pixelilor unei imagini RGB
void alloc_rgb(image *photo)
{
	photo->red = alloc_matrix(photo->width, photo->height);
	photo->green = alloc_matrix(photo->width, photo->height);
	photo->blue = alloc_matrix(photo->width, photo->height);
}

// functia citeste matricea pixelilor dintr-un fisier text
void read_ascii(image *photo, FILE *in)
{
	if (strcmp(photo->type, "P2") == 0) {
		//imagine grayscale
		photo->color = 0;
		alloc_gray(photo);
		for (int i = 0; i < photo->height; i++)
			for (int j = 0; j < photo->width; j++)
				fscanf(in, "%lf", &photo->gray[i][j]);
	} else {
		//imagine rgb
		photo->color = 1;
		alloc_rgb(photo);
		for (int i = 0; i < photo->height; i++)
			for (int j = 0; j < photo->width; j++) {
				fscanf(in, "%lf", &photo->red[i][j]);
				fscanf(in, "%lf", &photo->green[i][j]);
				fscanf(in, "%lf", &photo->blue[i][j]);
			}
	}
}

// functia citeste matricea pixelilor dintr-un fisier binar
void read_binary(image *photo, FILE *bin)
{
	//verific daca e grayscale
	if (strcmp(photo->type, "P5") == 0) {
		//imagine grayscale
		photo->color = 0;
		alloc_gray(photo);
		for (int i = 0; i < photo->height; i++)
			for (int j = 0; j < photo->width; j++) {
				unsigned char a;
				fread(&a, sizeof(unsigned char), 1, bin);
				photo->gray[i][j] = (double)a;
			}
	} else {
		//imagine RGB
		photo->color = 1;
		alloc_rgb(photo);
		for (int i = 0; i < photo->height; i++)
			for (int j = 0; j < photo->width; j++) {
				unsigned char a, b, c;
				fread(&a, sizeof(unsigned char), 1, bin);
				photo->red[i][j] = (double)a;
				fread(&b, sizeof(unsigned char), 1, bin);
				photo->green[i][j] = (double)b;
				fread(&c, sizeof(unsigned char), 1, bin);
				photo->blue[i][j] = (double)c;
			}
	}
}

// functia elibereaza memoria pentru o imagine
void free_memory(image *photo)
{
	if (strcmp(photo->type, "P2") == 0 || strcmp(photo->type, "P5") == 0) {
		// imaginea este grayscale
		for (int i = 0; i < photo->height; i++)
			free(photo->gray[i]);
		free(photo->gray);
	} else {
		// imaginea este RGB
		for (int i = 0; i < photo->height; i++) {
			free(photo->red[i]);
			free(photo->green[i]);
			free(photo->blue[i]);
		}
		free(photo->red);
		free(photo->green);
		free(photo->blue);
	}
}

// functia verifica daca exista o imagine incarcata
int is_loaded(image *photo)
{
	if (photo->loaded == 0) {
		printf("No image loaded\n");
		return 0;
	}

	// exista imagine incarcata
	return 1;
}

// functia interschimba 2 variabile de tip intreg
void swap_ints(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

// functia selecteaza intreaga imagine
void select_all(image *photo)
{
	if (is_loaded(photo) == 0)
		return;
	photo->x1 = 0;
	photo->y1 = 0;
	photo->x2 = photo->width;
	photo->y2 = photo->height;
}

// functia verifica daca mai exista vreun parametru de citit
int verify_parameter_existence(void)
{
	char ch;
	scanf("%c", &ch);

	if (ch == '\n') {
		// nu mai urmeaza de citit niciun parametru
		printf("Invalid command\n");
		return -1;
	}

	// mai este cel putin un parametru de citit
	return 1;
}

// functia citeste urmatorul parametru si verifica daca este valid
int next_parameter_validity(int *n)
{
	// verific daca mai e vreun parametru de citit
	if (verify_parameter_existence() == -1)
		return -1;

	char parameter[MAX_PARAMETER];
	scanf("%s", parameter);

	// verific daca primul caracter din parametrul introdus este o cifra sau '-'
	if (isdigit(parameter[0]) == 0 && parameter[0] != '-') {
		printf("Invalid command\n");
		return -1;
	}

	// verific daca parametrul e un numar
	for (unsigned int i = 1; i < strlen(parameter); i++)
		if (isdigit(parameter[i]) == 0) {
			printf("Invalid command\n");
			return -1;
		}

	// transform string-ul in intreg
	*n = atoi(parameter);

	// ultimul parametru introdus este valid si a fost transformat in intreg
	return 1;
}

// functia citeste parametrii care nu mai prezinta interes (surplus)
void read_dummy_input_select(void)
{
	char continue_command[MAX_COMMAND];
	// citeste urmatorii maxim 3 parametri
	for (int i = 0; i < 3; i++) {
		char ch;
		scanf("%c", &ch);
		// ma asigur ca parametrul exista
		if (ch != '\n')
			scanf("%s", continue_command);
		else
			break;
	}
}

// functia face o selectie asupra imaginii
void select_photo(image *photo)
{
	// verific daca nu este caracterul '\n' dupa SELECT
	char ch;
	scanf("%c", &ch);
	if (ch == '\n') {
		printf("Invalid command\n");
		return;
	}

	// vreau sa citesc ce se afla dupa SELECT, pana la urmatorul spatiu
	char continue_command[MAX_COMMAND];
	scanf("%s", continue_command);

	// verific daca formatul comenzii nu este chiar "SELECT ALL"
	if (strcmp(continue_command, "ALL") == 0) {
		select_all(photo);
		if (photo->loaded == 1)
			printf("Selected ALL\n");
		return;
	}

	int x1, y1, x2, y2;

	// verific daca a fost incarcata vreo imagine
	if (is_loaded(photo) == 0) {
		/* inainte de a iesi din functie,
		citesc restul parametrilor (y1, x2, y2), daca exista */
		read_dummy_input_select();
		return;
	}

	// verific ca primul parametru (care este deja citit) este numar

	// verific daca primul caracter din parametrul introdus este o cifra sau '-'
	if (isdigit(continue_command[0] == 0) && continue_command[0] != '-') {
		printf("Invalid command\n");
		return;
	}

	// verific daca parametrul e un numar
	for (unsigned int i = 1; i < strlen(continue_command); i++)
		if (isdigit(continue_command[i]) == 0) {
			printf("Invalid command\n");
			return;
		}
	x1 = atoi(continue_command);

	// citirea si verificarea validitatii parametrilor
	if (next_parameter_validity(&y1) == -1)
		return;
	if (next_parameter_validity(&x2) == -1)
		return;
	if (next_parameter_validity(&y2) == -1)
		return;

	if (x1 < 0 || x1 >= photo->width || y1 < 0 || y1 >= photo->height) {
		printf("Invalid set of coordinates\n");
		return;
	}

	if (x2 < 0 || x2 > photo->width || x1 == x2) {
		printf("Invalid set of coordinates\n");
		return;
	}

	if (y2 < 0 || y2 > photo->height || y1 == y2) {
		printf("Invalid set of coordinates\n");
		return;
	}
	if (x1 > x2)
		swap_ints(&x1, &x2);
	if (y1 > y2)
		swap_ints(&y1, &y2);

	// memorarea selectiei
	photo->x1 = x1;
	photo->y1 = y1;
	photo->x2 = x2;
	photo->y2 = y2;
	printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
}

// functia incarca o imagine in memorie
void load(image *photo)
{
	//verific daca exista o imagine deja incarcata
	if (photo->loaded == 1)
		free_memory(photo);

	char input_filename[MAX_FILENAME];
	scanf("%s", input_filename);
	FILE *in = fopen(input_filename, "r");
	// verific daca fisierul a fost deschis cu succes
	if (!in) {
		printf("Failed to load %s\n", input_filename);
		photo->loaded = 0;
		return;
	}
	//elimin comentariile
	skip_comm(in);
	fscanf(in, "%s", (*photo).type);
	skip_comm(in);
	fscanf(in, "%d%d", &(*photo).width, &(*photo).height);
	skip_comm(in);
	fscanf(in, "%d", &(*photo).max_value);
	skip_comm(in);

	if (strcmp(photo->type, "P2") == 0 || strcmp(photo->type, "P3") == 0) {
		read_ascii(photo, in);
	} else {
		// deschid fisier binar
		FILE *bin = fopen(input_filename, "rb");
		// verific daca fisierul a fost deschis cu succes
		if (!bin) {
			printf("Failed to load %s\n", input_filename);
			return;
		}
		// mut cursorul la inceputul matricei de pixeli
		fseek(bin, ftell(in), SEEK_SET);
		read_binary(photo, bin);
		fclose(bin);
	}
	fclose(in);
	photo->loaded = 1;
	printf("Loaded %s\n", input_filename);
	select_all(photo);
}

// functia decupeaza matricea pixelilor pentru o imagine grayscale
void crop_grayscale(image *photo)
{
	double **aux; //matricea noua
	int new_height, new_width; // noile dimensiuni ale matricei
	new_width = photo->x2 - photo->x1;
	new_height = photo->y2 - photo->y1;

	//aloc memorie pentru noua matrice
	aux = alloc_matrix(new_width, new_height);
	for (int i = 0; i < new_height; ++i)
		for (int j = 0; j < new_width; ++j)
			aux[i][j] = photo->gray[i + photo->y1][j + photo->x1];

	//eliberez memoria vechii matrice
	for (int i = 0; i < photo->height; ++i)
		free(photo->gray[i]);
	free(photo->gray);

	//ii aloc matricei vechi memorie in conformitate cu noile dimensiuni
	photo->gray = alloc_matrix(new_width, new_height);

	//copiez matricea auxiliara in vechea matrice
	for (int i = 0; i < new_height; ++i)
		for (int j = 0; j < new_width; ++j)
			photo->gray[i][j] = aux[i][j];

	//eliberez memoria matricei auxiliare
	for (int i = 0; i < new_height; ++i)
		free(aux[i]);
	free(aux);
}

// functia decupeaza matricea pixelilor pentru o imagine RGB
void crop_RGB(image *photo)
{
	double **aux_red; //matricea noua pt rosu
	double **aux_green; //matricea noua pt verde
	double **aux_blue; //matricea noua pt albastru
	int new_height, new_width; // noile dimensiuni ale matricelor
	new_width = photo->x2 - photo->x1;
	new_height = photo->y2 - photo->y1;

	//aloc memorie pentru noile matrice
	aux_red = alloc_matrix(new_width, new_height);
	aux_green = alloc_matrix(new_width, new_height);
	aux_blue = alloc_matrix(new_width, new_height);

	for (int i = 0; i < new_height; ++i)
		for (int j = 0; j < new_width; ++j) {
			aux_red[i][j] = (*photo).red[i + photo->y1][j + photo->x1];
			aux_green[i][j] = (*photo).green[i + photo->y1][j + photo->x1];
			aux_blue[i][j] = (*photo).blue[i + photo->y1][j + photo->x1];
		}

	//eliberez memoria vechilor matrice
	for (int i = 0; i < photo->height; ++i) {
		free(photo->red[i]);
		free(photo->green[i]);
		free(photo->blue[i]);
	}
	free(photo->red);
	free(photo->green);
	free(photo->blue);

	//le aloc matricelor vechi memorie in conformitate cu noile dimensiuni
	(*photo).red = alloc_matrix(new_width, new_height);
	(*photo).green = alloc_matrix(new_width, new_height);
	(*photo).blue = alloc_matrix(new_width, new_height);

	//copiez matricele auxiliare in vechea matrice
	for (int i = 0; i < new_height; ++i)
		for (int j = 0; j < new_width; ++j) {
			photo->red[i][j] = aux_red[i][j];
			photo->green[i][j] = aux_green[i][j];
			photo->blue[i][j] = aux_blue[i][j];
		}

	//eliberez memoria matricelor auxiliare
	for (int i = 0; i < new_height; ++i) {
		free(aux_red[i]);
		free(aux_green[i]);
		free(aux_blue[i]);
	}
	free(aux_red);
	free(aux_green);
	free(aux_blue);
}

// functia decupeaza imaginea in conformitate cu selectia
void crop(image *photo)
{
	// verific daca exista vreo imagine incarcata
	if (is_loaded(photo) == 0)
		return;

	// verific daca imaginea e grayscale sau RGB
	if (photo->color == 0)
		crop_grayscale(photo);
	else
		crop_RGB(photo);

	// modific dimensiunile imaginii
	photo->width = photo->x2 - photo->x1;
	photo->height = photo->y2 - photo->y1;

	printf("Image cropped\n");
	select_all(photo);
}

// functia verifica daca parametrul y este valid
int is_y_valid(int y)
{
	// verific daca y se afla in intervalul [2, 256]
	if (y < 2 || y > 256) {
		printf("Invalid set of parameters\n");
		return -1;
	}

	// verific daca y este putere a lui 2
	while (y != 1) {
		if (y % 2 == 1) {
			printf("Invalid set of parameters\n");
			return -1;
		}
		y = y / 2;
	}
	return 1;
}

// functia verifica daca imaginea este grayscale
int is_grayscale(image *photo)
{
	if (photo->color == 1) { // imaginea este RGB
		printf("Black and white image needed\n");
		return 0;
	}
	return 1;
}

// functia realizeaza histograma imaginii
void histogram(image *photo)
{
	// citesc toti parametrii de dupa comanda "HISTOGRAM"
	char ch;
	int first_parameter = 0; // (0) primul parametru citit, (1) necitit
	int second_parameter = 0; // (0) al doilea parametru citit, (1) necitit
	int dummy = -1; // surplus de parametri
	int x, y;
	scanf("%c", &ch);
	// citeste parametri pana se trece la rand nou
	while (ch != '\n') {
		if (first_parameter == 0) {
			scanf("%d", &x);
			first_parameter = 1;
		} else {
			if (second_parameter == 0) {
				scanf("%d", &y);
				second_parameter = 1;
			} else {
				scanf("%d", &dummy);
			}
		}
		scanf("%c", &ch);
	}
	// verific daca imaginea a fost incarcata
	if (is_loaded(photo) == 0)
		return;

	// verific validitatea comenzii
	if (first_parameter == 0 || second_parameter == 0 || dummy != -1) {
		printf("Invalid command\n");
		return;
	}

	// verific validitatea parametrului y
	if (is_y_valid(y) == -1)
		return;

	// verific daca imaginea este grayscale
	if (is_grayscale(photo) == 0)
		return;

	// construiesc vectorul de frecventa
	int freq[FREQ_MAX] = {0};
	for (int i = 0; i < (*photo).height; ++i)
		for (int j = 0; j < (*photo).width; ++j)
			freq[(int)(*photo).gray[i][j]]++;

	// construiesc un nou vector de frecventa, dar impartit in exact y bin-uri
	int *bin_freq;
	bin_freq = (int *)calloc(y, sizeof(int));
	// programare defensiva
	if (!bin_freq) {
		fprintf(stderr, "Calloc failed\n");
		return;
	}
	/* in al doilea vector se trec sumele elementelor
	din fiecare bin_interval din primul vector */
	int bin_interval = 256 / y;
	for (int i = 0; i <= 256 - bin_interval; i = i + bin_interval) {
		int sum = 0;
		for (int cnt = 0; cnt < bin_interval; ++cnt)
			sum = sum + freq[i + cnt];
		bin_freq[i / bin_interval] = sum;
	}
	// caut elementul maxim din al doilea vector de frecventa
	int freq_max = 0;
	for (int i = 0; i < y; ++i)
		if (freq_max < bin_freq[i])
			freq_max = bin_freq[i];

	// construiesc histograma bazat pe bin_freq[]
	for (int i = 0; i < y; ++i) {
		int star_number = (x * bin_freq[i]) / freq_max;
		printf("%d\t", star_number);
		printf("|\t");
		for (int cnt = 0; cnt < star_number; ++cnt)
			printf("*");
		printf("\n");
	}
	free(bin_freq);
}

double clamp(double x)
{
	if (x < 0)
		x = 0;
	if (x > 255)
		x = 255;
	return x;
}

// functie care rotunjeste o variabila de tip double
int my_round(double x)
{
	if ((int)x == (int)(x - 0.5))
		return (int)x + 1;
	return (int)x;
}

// functia aplica un filtru pe selectia unei matrice
void apply_matrix(image *photo, int effect, double ***a)
{
	double edge[4][4] = {
		{-1, -1, -1},
		{-1, 8, -1},
		{-1, -1, -1}};
	double sharp[4][4] = {
		{0, -1,  0},
		{-1, 5, -1},
		{0, -1,  0}};
	double blur[4][4] = {
		{1.0 / 9, 1.0 / 9, 1.0 / 9},
		{1.0 / 9, 1.0 / 9, 1.0 / 9},
		{1.0 / 9, 1.0 / 9, 1.0 / 9}};
	double gauss[4][4] = {
		{1.0 / 16, 2.0 / 16, 1.0 / 16},
		{2.0 / 16, 4.0 / 16, 2.0 / 16},
		{1.0 / 16, 2.0 / 16, 1.0 / 16}};
	int selected_width = photo->x2 - photo->x1;
	int selected_height = photo->y2 - photo->y1;
	int h = photo->height;
	int w = photo->width;
	// aloc memorie pentru o matrice auxiliara
	double **new;
	new = alloc_matrix(selected_width, selected_height);
	for (int i = (*photo).y1; i < (*photo).y2; ++i) {
		for (int j = (*photo).x1; j < (*photo).x2; ++j) {
			double sum = 0;
			// verific sa nu fiu pe marginile matricei
			if (i == 0 || i == h - 1 || j == 0 || j == w - 1)
				continue;
			// aplic kernel-ul in functie de filtrul dorit
			for (int row = -1; row <= 1; ++row)
				for (int col = -1; col <= 1; ++col)
					switch (effect) {
					case 1:
						sum += (*a)[i + row][j + col] * edge[1 + row][1 + col];
						break;
					case 2:
						sum += (*a)[i + row][j + col] * sharp[1 + row][1 + col];
						break;
					case 3:
						sum += (*a)[i + row][j + col] * blur[1 + row][1 + col];
						break;
					case 4:
						sum += (*a)[i + row][j + col] * gauss[1 + row][1 + col];
						break;
					}
			new[i - photo->y1][j - photo->x1] = clamp(sum);
		}
	}

	//fac modificarile in matricea initiala
	for (int i = photo->y1; i < photo->y2; ++i)
		for (int j = photo->x1; j < photo->x2; ++j) {
			if (i != 0 && i != photo->height - 1)
				if (j != 0 && j != photo->width - 1)
					(*a)[i][j] = new[i - photo->y1][j - photo->x1];
		}

	// eliberez memoria matricei auxiliare
	for (int i = 0; i < selected_height; ++i)
		free(new[i]);
	free(new);
}

// functia aplica filtru asupra selectiei imaginii
void apply_filter(image *photo)
{
	char continue_command[MAX_COMMAND];
	// verific daca nu cumva a fost introdusa comanda APPLY\n
	char ch;
	scanf("%c", &ch);

	// verific daca a fost incarcata vreo poza
	if (is_loaded(photo) == 0) {
		/* citesc parametrul comenzii APPLY (daca exista),
		chiar daca nu il voi folosi */
		if (ch == ' ')
			scanf("%s", continue_command);
		return;
	}

	// verific daca lipseste parametrul
	if (ch == '\n') {
		printf("Invalid command\n");
		return;
	}

	// citeste parametrul comenzii "APPLY"
	scanf("%s", continue_command);

	// verific daca imaginea este grayscale
	if (photo->color == 0) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}

	int effect = 0; // codificarea parametrului
	if (strcmp(continue_command, "EDGE") == 0)
		effect = 1;
	else
		if (strcmp(continue_command, "SHARPEN") == 0)
			effect = 2;
		else
			if (strcmp(continue_command, "BLUR") == 0)
				effect = 3;
			else
				if (strcmp(continue_command, "GAUSSIAN_BLUR") == 0)
					effect = 4;
	if (effect == 0) {
		printf("APPLY parameter invalid\n");
		return;
	}
	apply_matrix(photo, effect, &photo->red);
	apply_matrix(photo, effect, &photo->green);
	apply_matrix(photo, effect, &photo->blue);

	printf("APPLY %s done\n", continue_command);
}

// functia egalizeaza imaginea
void equalize(image *photo)
{
	// verific daca imaginea a fost incarcata
	if (is_loaded(photo) == 0)
		return;

	// verific daca imaginea este grayscale
	if (is_grayscale(photo) == 0)
		return;

	double area = (double)(photo->width * photo->height);

	// construiesc vectorul de frecventa
	int freq[FREQ_MAX] = {0};
	for (int i = 0; i < photo->height; ++i)
		for (int j = 0; j < photo->width; ++j) {
			int x = (int)photo->gray[i][j];
			freq[x]++;
		}

	/* construiesc un vector in care se va retine pe fiecare pozitie
	suma aparitiilor pixelilor de dinaintea sa inclusiv */
	int sum_H[FREQ_MAX] = {0};
	int sum = 0;
	for (int i = 0; i <= 255; ++i) {
		sum = sum + freq[i];
		sum_H[i] = sum;
	}

	// se modifica matricea conform formulei
	for (int i = 0; i < photo->height; ++i)
		for (int j = 0; j < photo->width; ++j) {
			double k = photo->gray[i][j];
			photo->gray[i][j] = clamp((255.0 / area) * (double)sum_H[(int)k]);
			photo->gray[i][j] = my_round(photo->gray[i][j]);
		}

	printf("Equalize done\n");
}

// functie de salvare in format text
void save_txt(image *photo, FILE *out)
{
	if (photo->color == 0) // imagine grayscale
		for (int i = 0; i < photo->height; ++i) {
			for (int j = 0; j < photo->width; ++j)
				fprintf(out, "%d ", (int)my_round((*photo).gray[i][j]));
			fprintf(out, "\n");
		} else { // imagine RGB
			for (int i = 0; i < photo->height; ++i) {
				for (int j = 0; j < photo->width; ++j) {
					fprintf(out, "%d ", (int)my_round((*photo).red[i][j]));
					fprintf(out, "%d ", (int)my_round((*photo).green[i][j]));
					fprintf(out, "%d ", (int)my_round((*photo).blue[i][j]));
				}
				fprintf(out, "\n");
			}
		}
		fclose(out);
}

// functie de salvare binar
void save_binary(image *photo, FILE *out)
{
	if (photo->color == 0) // grayscale
		for (int i = 0; i < photo->height; ++i)
			for (int j = 0; j < photo->width; ++j) {
				unsigned char a;
				a = my_round(photo->gray[i][j]);
				fwrite(&a, sizeof(unsigned char), 1, out);
			}
	else // RGB
		for (int i = 0; i < photo->height; ++i)
			for (int j = 0; j < photo->width; ++j) {
				unsigned char a, b, c;
				a = my_round((*photo).red[i][j]);
				fwrite(&a, sizeof(unsigned char), 1, out);
				b = my_round((*photo).green[i][j]);
				fwrite(&b, sizeof(unsigned char), 1, out);
				c = my_round((*photo).blue[i][j]);
				fwrite(&c, sizeof(unsigned char), 1, out);
			}
	fclose(out);
}

// functia salveaza imaginea
void save(image *photo)
{
	char filename[MAX_FILENAME];
	scanf("%s", filename);

	//contor care indica formatul de salvare: (0) text, (1) binar
	int save_format;

	char continue_command[MAX_COMMAND];
	char ch;
	scanf("%c", &ch);
	if (ch == ' ') { // urmeaza paramterul "ascii" sau caracterul '\n'
		char ch2;
		scanf("%c", &ch2);
		if (ch2 == '\n') { // nu exista parametrul "ascii"
			save_format = 1;
		} else { // urmeaza parametrul "ascii"
			scanf("%s", continue_command);
			save_format = 0;
		}
	} else { // trece pe rand nou, deci nu mai asteapta niciun parametru
		save_format = 1;
	}

	if (is_loaded(photo) == 0)
		return;

	/* deschid fisierul in format text pentru a scrie magic-word-ul,
	latimea, inaltimea si valoarea maxima */
	FILE *out;
	out = fopen(filename, "wt");
	// Verific daca a esuat deschiderea fisierului
	if (!out) {
		fprintf(stderr, "Failed to open %s\n", filename);
		return;
	}

	if (save_format == 0) // ascii
		if ((*photo).color == 0) //grayscale
			fprintf(out, "%s\n", "P2");
		else
			fprintf(out, "%s\n", "P3"); // RGB
	else //binar
		if ((*photo).color == 0) // grayscale
			fprintf(out, "%s\n", "P5");
		else
			fprintf(out, "%s\n", "P6"); // RGB
	fprintf(out, "%d %d\n", photo->width, photo->height);
	fprintf(out, "%d\n", photo->max_value);

	if (save_format == 0) { // ascii
		save_txt(photo, out);
	} else { // binar
		fclose(out);
		// deschid fisierul in format binary append
		FILE *out = fopen(filename, "ab");
		if (!out) {
			fprintf(stderr, "Failed to open %s\n", filename);
			return;
		}
		save_binary(photo, out);
	}
	printf("Saved %s\n", filename);
}

// functie de iesire din program
void exit_program(image *photo, int *exit_flag)
{
	// verific daca exista vreo imagine incarcata
	if (is_loaded(photo) == 0) {
		(*exit_flag) = 0;
		return;
	}
	free_memory(photo);
	(*exit_flag) = 0;
}

void swap_doubles(double *a, double *b)
{
	double tmp = *a;
	*a = *b;
	*b = tmp;
}

// functia verifica daca a fost selectata toata imaginea
int is_selected_all(image *photo)
{
	if (photo->x2 - photo->x1 == photo->width)
		if (photo->y2 - photo->y1 == photo->height)
			return 1;

	// nu a fost selectata toata imaginea
	return 0;
}

// functia verifica daca selectia este patratica (de dimensiune n*n)
int is_square(image *photo)
{
	if (photo->x2 - photo->x1 != photo->y2 - photo->y1) {
		printf("The selection must be square\n");
		return 0;
	}

	return 1; // selectie corespunzatoare
}

// verific daca unghiul introdus este valid
int is_valid_angle(int angle)
{
	// verific dac unghiul introdus se afla in intervalul [-360, 360]
	if (angle > 0)
		if (angle > 360) {
			printf("Unsupported rotation angle\n");
			return 0;
		}
	if (angle < 0)
		if (angle < -360) {
			printf("Unsupported rotation angle\n");
			return 0;
		}

	// verific daca unghiul este multiplu de 90
	if (angle % 90 != 0) {
		printf("Unsupported rotation angle\n");
		return 0;
	}

	return 1; // unghi valid
}

// functia transforma unghiurile negative in corespondentul lor pozitiv
void make_angle_positive(int *angle)
{
	if ((*angle) == 360)
		(*angle) = 0;

	if ((*angle) < 0)
		(*angle) = (*angle) + 360;
}

// functie de rotatie a unei selectii patratice cu 90 de grade la dreapta
void rotate_square(image *photo, double ***a)
{
	/* este suficient sa retinem o singura dimensiune pentru
	matricea auxiliara, caci selectia este oricum patratica,
	deci inaltimea va fi egala cu latimea */
	int selected_width = photo->x2 - photo->x1;

	// consideram o matrice auxiliara
	double **aux;
	aux = alloc_matrix(selected_width, selected_width);

	// se roteste matricea selectata la dreapta cu 90 de grade
	for (int i = 0; i < selected_width; ++i)
		for (int j = 0; j < selected_width; ++j)
			aux[j][selected_width - 1 - i] = (*a)[i + photo->y1][j + photo->x1];

	// in matricea mare, se inlocuieste matricea selectata initial cu cea rotita
	for (int i = 0; i < selected_width; ++i)
		for (int j = 0; j < selected_width; ++j)
			(*a)[i + (*photo).y1][j + (*photo).x1] = aux[i][j];

	// se elibereaza memoria matricei auxiliare
	for (int i = 0; i < selected_width; ++i)
		free(aux[i]);
	free(aux);
}

// functie de rotatie a intregii imagini cu 90 de grade la dreapta
void rotate_entire_image(image *photo, double ***a)
{
	// consideram o matrice auxiliara, cu dimensiunile inversate
	double **aux;
	aux = alloc_matrix(photo->height, photo->width);

	for (int i = 0; i < photo->height; ++i)
		for (int j = 0; j < photo->width; ++j)
			aux[j][photo->height - 1 - i] = (*a)[i][j];

	// Odata cu rotatia, dimensiunile matricei trebuie inversate

	// eliberez memoria pentru matricea a cu dimensiunile(w x h)
	for (int i = 0; i < photo->height; i++)
		free((*a)[i]);
	free(*a);

	// aloc memorie pentru matricea a cu dimensiunile inversate (h x w)
	(*a) = alloc_matrix(photo->height, photo->width);

	// modificarea matricei initiale
	for (int i = 0; i < photo->width; ++i)
		for (int j = 0; j < photo->height; ++j)
			(*a)[i][j] = aux[i][j];

	// eliberez memoria matricei auxiliare
	for (int i = 0; i < photo->width; ++i)
		free(aux[i]);
	free(aux);
}

// functie de rotatie dupa un anumit unghi
void rotate(image *photo)
{
	int angle, copy_angle;
	scanf("%d", &angle);

	//voi avea nevoie de o copie a unghiului de rotatie
	copy_angle = angle;

	//verific daca exista vreo imagine incarcata
	if (is_loaded(photo) == 0)
		return;

	/* daca nu a fost selectata intreaga imagine,
	verific daca selectia facuta este patratica */
	if (is_selected_all(photo) == 0)
		if (is_square(photo) == 0)
			return;

	//verific validitatea unghiului introdus
	if (is_valid_angle(copy_angle) == 0)
		return;

	// transforma unghiul in echivalentul lui pozitiv
	make_angle_positive(&copy_angle);

	while (copy_angle != 0) {
		if (is_selected_all(photo) == 1) {
			if (photo->color == 0) { // grayscale
				rotate_entire_image(photo, &photo->gray);
			} else { // RGB
				rotate_entire_image(photo, &photo->red);
				rotate_entire_image(photo, &photo->green);
				rotate_entire_image(photo, &photo->blue);
			}
			/* daca se roteste o imagine care nu este patratica,
			trebuie sa inversam latimea si inaltimea pozei */
			swap_ints(&photo->width, &photo->height);
			select_all(photo);
		} else {
			if (photo->color == 0) { //grayscale
				rotate_square(photo, &photo->gray);
			} else { //RGB
				rotate_square(photo, &photo->red);
				rotate_square(photo, &photo->green);
				rotate_square(photo, &photo->blue);
			}
		}
		copy_angle = copy_angle - 90;
	}

	printf("Rotated %d\n", angle);
}

// functie care anunta invaliditatea unei comenzi si citeste ii parametrii
void invalid_command(void)
{
	printf("Invalid command\n");
	// daca mai exista parametri dupa comanda invalida, ii citeste
	char ch;
	scanf("%c", &ch);
	while (ch != '\n')
		scanf("%c", &ch);
}
