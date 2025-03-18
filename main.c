/* SAFRONII VEACESLAV - 315CD */
#include "functions.h"

int main(int argc, char* argv[])
{
	/* Deschiderea fisierelor de input si output, date ca argumente */
	if (argc == 1) return -1;
	FILE *input;
	FILE *output;
	if (argc == 5) {
		input = fopen(argv[3], "rb");
		output = fopen(argv[4], "w");
	} else if (argc == 4) {
		input = fopen(argv[2], "rb");
		output = fopen(argv[3], "w");
	} else {
		return -1;
	}
	/* ----------------------------------------------------------- */

	if (!input) return -1;
	if (!output) return -1;

	/* Verificarea optiunilor alese la executia programului */
	if (!strcmp(argv[1], "-c1") || !strcmp(argv[1], "-c2")) {
		/* Partea comuna pentru cerintele 1 si 2 */
		unsigned factor = atoi(argv[2]);
		unsigned size = 0;
		RGB **grid = NULL;
		/* Citirea fisierului PPM */
		readFile(input, (int*)&size, &grid);
		/* Calcularea valorilor medii ale
		culorilor RGB, pentru intreaga imagine */
		unsigned red = 0;
		unsigned green = 0;
		unsigned blue = 0;
		average_value(grid, 0, 0, (int)size, &red, &green, &blue);

		/* Crearea arborelui */
		Node *root = createNode();
		root = createTree(root, factor, grid, 0, 0,
				(int)size, red, green, blue);

		if (!strcmp(argv[1], "-c1")) {
					/* Cerinta 1 */
			/* Determinarea nivelurilor din arbore */
			int nr_levels = get_levels(root);
			/* Determinarea numarului de frunze */
			int nr_leaves = 0;
			undivised_blocks(root, &nr_leaves);
			/* Determinarea dimensiunii celui mai mare
			bloc nedivizat din arbore */
			int min_lvl = __INT_MAX__;
			unsigned dim = 0;
			biggest_undivised_block(root, 0, &min_lvl, &dim);
			/* Afisarea informatiilor intr-un fisier text */
			fprintf(output, "%d\n", nr_levels);
			fprintf(output, "%d\n", nr_leaves);
			fprintf(output, "%u\n", dim);
		} else {
				/* Cerinta 2 */
			/* Comprimarea imaginii */
			compress_image(output, root, size);
		}
		/* Dealocarea memoriei pentru arbore,
		si pentru matricea de pixeli */
		freeTree(&root);
		for(int i = 0; i < (int) size; i++) {
			free(grid[i]);
		}
		free(grid);

	} else if (!strcmp(argv[1], "-d")) {
			/* Cerinta 3 */
		/* Decomprimarea imaginii */
		decompress_image(input, output);
	}

	fclose(input);
	fclose(output);
}
