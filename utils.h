// STEFAN MIRUNA ANDREEA 314CA

#pragma once

// functie pentru ignorarea comentariilor
void skip_comm(FILE * in);

//programare defensiva
void defensive_programming(double **a);

// functie care aloca memorie pentru o matrice
double **alloc_matrix(int width, int height);

//functia aloca memorie pentru matricea pixelilor unei imagini grayscale
void alloc_gray(image *photo);

//functia aloca memorie pentru matricea pixelilor unei imagini RGB
void alloc_rgb(image *photo);

// functia citeste matricea pixelilor dintr-un fisier text
void read_ascii(image *photo, FILE *in);

// functia citeste matricea pixelilor dintr-un fisier binar
void read_binary(image *photo, FILE *bin);

// functia elibereaza memoria pentru o imagine
void free_memory(image *photo);

// functia verifica daca exista o imagine incarcata
int is_loaded(image *photo);

// functia interschimba 2 variabile de tip intreg
void swap_ints(int *a, int *b);

// functia selecteaza intreaga imagine
void select_all(image *photo);

// functia verifica daca mai exista vreun parametru de citit
int verify_parameter_existence(void);

// functia citeste urmatorul parametru si verifica daca este valid
int next_parameter_validity(int *n);

// functia citeste parametrii care nu mai prezinta interes (surplus)
void read_dummy_input_select(void);

// functia face o selectie asupra imaginii
void select_photo(image *photo);

// functia incarca o imagine in memorie
void load(image *photo);

// functia decupeaza matricea pixelilor pentru o imagine grayscale
void crop_grayscale(image *photo);

// functia decupeaza matricea pixelilor pentru o imagine RGB
void crop_RGB(image *photo);

// functia decupeaza imaginea in conformitate cu selectia
void crop(image *photo);

// functia verifica daca parametrul y este valid
int is_y_valid(int y);

// functia verifica daca imaginea este grayscale
int is_grayscale(image *photo);

// functia realizeaza histograma imaginii
void histogram(image *photo);

double clamp(double x);

// functie care rotunjeste o variabila de tip double
int my_round(double x);

// functia aplica un filtru pe selectia unei matrice
void apply_matrix(image *photo, int effect, double ***a);

// functia aplica filtru asupra selectiei imaginii
void apply_filter(image *photo);

// functia egalizeaza imaginea
void equalize(image *photo);

// functie de salvare in format text
void save_txt(image *photo, FILE *out);

// functie de salvare binar
void save_binary(image *photo, FILE *out);

// functia salveaza imaginea
void save(image *photo);

// functie de iesire din program
void exit_program(image *photo, int *exit_flag);

void swap_doubles(double *a, double *b);

// functia verifica daca a fost selectata toata imaginea
int is_selected_all(image *photo);

// functia verifica daca selectia este patratica (de dimensiune n*n)
int is_square(image *photo);

// verific daca unghiul introdus este valid
int is_valid_angle(int angle);

// functia transforma unghiurile negative in corespondentul lor pozitiv
void make_angle_positive(int *angle);

// functie de rotatie a unei selectii patratice cu 90 de grade la dreapta
void rotate_square(image *photo, double ***a);

// functie de rotatie a intregii imagini cu 90 de grade la dreapta
void rotate_entire_image(image *photo, double ***a);

// functie de rotatie dupa un anumit unghi
void rotate(image *photo);

// functie care anunta invaliditatea unei comenzi si citeste ii parametrii
void invalid_command(void);
