//STEFAN MIRUNA ANDREEA 314CA
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

int main(void)
{
	image photo;
	char command[MAX];
	photo.loaded = 0;
	int exit_flag = 1;

	while (exit_flag == 1) {
		// citeste primul cuvant din comanda
		scanf("%s", command);
		if (strcmp(command, "LOAD") == 0)
			load(&photo);
		else if (strcmp(command, "SELECT") == 0)
			select_photo(&photo);
		else if (strcmp(command, "CROP") == 0)
			crop(&photo);
		else if (strcmp(command, "APPLY") == 0)
			apply_filter(&photo);
		else if (strcmp(command, "SAVE") == 0)
			save(&photo);
		else if (strcmp(command, "EXIT") == 0)
			exit_program(&photo, &exit_flag);
		else if (strcmp(command, "HISTOGRAM") == 0)
			histogram(&photo);
		else if (strcmp(command, "EQUALIZE") == 0)
			equalize(&photo);
		else if (strcmp(command, "ROTATE") == 0)
			rotate(&photo);
		else
			invalid_command();
	}
	return 0;
}
